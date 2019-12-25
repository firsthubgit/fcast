// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package pedia.cast.pall;

import android.Manifest;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.util.Log;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import com.tencent.tcd.bean.PingTask;
import com.tencent.tcd.bean.TCDAbilityConfig;
import com.tencent.tcd.bean.TCDPrivateConfig;
import com.tencent.tcd.bean.TCDUser;
import com.tencent.tcd.sender.TCDCastConfig;
import com.tencent.tcd.sender.TCDCastPermissionInfo;
import com.tencent.tcd.sender.TCDEngineSender;
import com.tencent.tcd.sender.TCDRecoveryInfo;
import com.tencent.tcd.sender.TCDSenderConfig;
import com.tencent.tcd.sender.TCDSenderListener;
import io.flutter.plugin.common.EventChannel.EventSink;
import io.flutter.plugin.common.EventChannel.StreamHandler;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry;
import java.util.List;

/**
 * WecastPlugin
 */
public class WecastPlugin implements MethodCallHandler, StreamHandler {
  private final PluginRegistry.Registrar registrar;
  private final MethodChannel channel;
  private boolean inited = false;
  private BroadcastReceiver chargingStateChangeReceiver;

  private Delegate delegate;

  private static final int REQUEST_MEDIA_PROJECTION = 10002;
  private static final String TAG = "tcdlistener";

  WecastPlugin(PluginRegistry.Registrar registrar, MethodChannel channel) {
    this.registrar = registrar;
    this.channel = channel;

    this.delegate = new Delegate();
    registrar.addActivityResultListener(this.delegate);
  }

  /**
   * Plugin registration.
   */
  public static void registerWith(PluginRegistry.Registrar registrar) {
    final MethodChannel methodChannel =
        new MethodChannel(registrar.messenger(), "wecast");
    final WecastPlugin instance = new WecastPlugin(registrar, methodChannel);
    methodChannel.setMethodCallHandler(instance);
  }

  @Override
  public void onMethodCall(MethodCall call, Result result) {
    if (!inited) {
      init();
    }

    final TCDEngineSender sender = TCDEngineSender.getInstance();

    if (call.method.equals("getPlatformVersion")) {
      // result(
      //     [@"macOS " stringByAppendingString:[[NSProcessInfo processInfo]
      //                                            operatingSystemVersionString]]);
    } else if (call.method.equals("queryPermission")) {
      if (hasPermission())
        result.success(true);
      else {
        delegate.current = new Current(REQUEST_MEDIA_PROJECTION, result);
        queryPermission();
      }
    } else if (call.method.equals("shutdown")) {
      sender.stopTCDEngine();
      result.success(null);
    } else if (call.method.equals("init")) {
      TCDAbilityConfig abilityConfig = new TCDAbilityConfig();
      abilityConfig.usingMirror = call.argument("mirror");
      sender.setAbilityConfig(abilityConfig);

      TCDSenderConfig senderConfig = new TCDSenderConfig();
      senderConfig.corpId = call.argument("corpId");
      senderConfig.corpAuth = call.argument("corpAuth");
      senderConfig.nickname = call.argument("nickName");

      TCDPrivateConfig privateConfig = new TCDPrivateConfig();
      privateConfig.url = call.argument("privateUrl");
      sender.setPrivateConfig(privateConfig);

      sender.startTCDEngine(senderConfig);
      result.success(null);
    } else if (call.method.equals("startCast")) {
      TCDCastConfig config = new TCDCastConfig();
      config.pin = call.toString();
      // TODO: permissionCode
      sender.startCast(config, new TCDCastPermissionInfo());
      result.success(null);
    } else if (call.method.equals("stopCast")) {
      sender.stopCast();
      result.success(null);
    } else if (call.method.equals("pauseCast")) {
      // sender.pauseCast();
      result.success(null);
    } else if (call.method.equals("resumeCast")) {
      // sender.resumeCast();
      result.success(null);
    } else if (call.method.equals("startNetCheck")) {
      sender.startCheckNetwork();
      result.success(null);
    } else {
      result.notImplemented();
    }
  }

  void init() {
    if (inited)
      return;

    inited = true;

    TCDEngineSender.getInstance().setListener(new SenderListener(channel));
  }

  boolean hasPermission() {
    Activity activity = registrar.activity();
    return ContextCompat.checkSelfPermission(
               activity, Manifest.permission.WRITE_EXTERNAL_STORAGE)
        == PackageManager.PERMISSION_GRANTED
        && ContextCompat.checkSelfPermission(
               activity, Manifest.permission.INTERNET)
        == PackageManager.PERMISSION_GRANTED;
  }

  void queryPermission() {
    Activity activity = registrar.activity();

    ActivityCompat.requestPermissions(activity,
        new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.INTERNET},
        REQUEST_MEDIA_PROJECTION);
  }

  @Override
  public void onListen(Object arguments, EventSink events) {
    // chargingStateChangeReceiver = createChargingStateChangeReceiver(events);
    // registrar.context().registerReceiver(chargingStateChangeReceiver,
    //     new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
  }

  @Override
  public void onCancel(Object arguments) {
    // registrar.context().unregisterReceiver(chargingStateChangeReceiver);
    // chargingStateChangeReceiver = null;
  }

  private final class SenderListener implements TCDSenderListener {
    private final MethodChannel channel;

    SenderListener(MethodChannel channel) {
      this.channel = channel;
    }

    public void onEngineStarted(int errorCode, TCDUser selfInfo) {
      Log.d(
          TAG, "onEngineStarted errorCode = $errorCode, selfInfo = $selfInfo");
      this.channel.invokeMethod("engineStarted", errorCode);
    }

    public void onAuthInfoExpired() {
      Log.d(TAG, "onAuthInfoExpired");
    }

    public void onUserChanged(
        int changeType, List<TCDUser> changeList, List<TCDUser> totalList) {
      Log.d(TAG,
          "onUserChanged changeType = $changeType, changeList = $changeList, totalList = $totalList");
    }

    public void onNetStateChanged(boolean disconnected) {
      Log.d(TAG, "onNetStateChanged disconnected = $disconnected");
    }

    public void onCastStopped(int reason) {
      Log.d(TAG, "onCastStopped reason = $reason");
    }

    public void onTipsUpdated(String content) {}

    public void onNetworkCheckProgress(
        String url, String description, int progress, int totalSize) {
      Log.d(TAG,
          "onNetworkCheckProgress url = $url,description = $description, progress = $progress, totalSize = $totalSize");
    }

    public void onNetworkCheckFinished(List<PingTask> items) {
      Log.d(TAG, "onNetworkCheckFinished items = $items");
    }

    public void onCastStarted(int retCode) {
      Log.d(TAG, "onCastStarted retCode = $retCode");
    }

    public void onCastAdded(int retCode, TCDCastConfig config) {
      Log.d(TAG, "onCastAdded retCode = $retCode, config = $config");
    }

    public void onCastStateChanged(int retCode, int castState) {
      Log.d(
          TAG, "onCastStateChanged retCode = $retCode,castState = $castState");
    }

    public void onRecoveryNotify(TCDRecoveryInfo info){
      Log.d(TAG, "onRecoveryNotify info = $info");
    }

    public void onRecoveryCompleted(int retCode, TCDRecoveryInfo info) {
      Log.d(TAG, "onRecoveryCompleted retCode = $retCode, info = $info");
    }

    public void onKickOut() {}

    public void onStreamInfoUpdated() {}
  }

  public static final class Current {
    public int request;
    public Result result;

    Current(int request, Result result) {
      this.request = request;
      this.result = result;
    }
  }

  public static final class Delegate
      implements PluginRegistry.ActivityResultListener {
    private boolean pendingOperation = false;

    public Current current;

    void reply(Object object, int expectRequest) {
      if (current != null && expectRequest == current.request)
        current.result.success(object);

      current = null;
    }

    @Override
    public boolean onActivityResult(
        int requestCode, int resultCode, Intent data) {
      if (pendingOperation)
        return false;
      switch (requestCode) {
        case REQUEST_MEDIA_PROJECTION:
          if (resultCode != Activity.RESULT_OK)
            // TODO: check current
            current.result.error("ScreenCaptureAccess", "TODO", null);

          // if (resultCode !=0 && data != null)

          return true;
      }
      return false;
    }
  }
}
