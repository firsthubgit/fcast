package pedia.cast.pall;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.projection.MediaProjectionManager;
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
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * WecastPlugin
 * TODO: base from FlutterPlugin, ActivityAware
 */
public class WecastPlugin implements MethodCallHandler, StreamHandler {
  private final PluginRegistry.Registrar registrar;
  private final MethodChannel channel;
  private boolean inited = false;
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
    final TCDEngineSender sender = TCDEngineSender.getInstance();

    if (!inited) {
      inited = true;

      // init tcd.so ...
      sender.init(registrar.context());

      // event
      sender.setListener(new SenderListener(channel));
    }

    if (call.method.equals("getPlatformVersion")) {
      result.success(3.2);
    } else if (call.method.equals("queryPermission")) {
      if (hasPermission())
        result.success(true);
      else {
        delegate.current =
            new Current(REQUEST_MEDIA_PROJECTION, result, null, null, null);
        queryPermission();
      }
    } else if (call.method.equals("shutdown")) {
      sender.stopTCDEngine();
      result.success(null);
    } else if (call.method.equals("init")) {
      TCDAbilityConfig abilityConfig = new TCDAbilityConfig();
      abilityConfig.usingMirror = (Boolean) call.argument("mirror");
      sender.setAbilityConfig(abilityConfig);

      TCDPrivateConfig privateConfig = new TCDPrivateConfig();
      privateConfig.url = (String) call.argument("privateUrl");
      sender.setPrivateConfig(privateConfig);

      TCDSenderConfig senderConfig = new TCDSenderConfig();
      senderConfig.corpId = call.argument("corpId");
      senderConfig.corpAuth = call.argument("corpAuth");
      senderConfig.nickname = call.argument("nickName");
      sender.startTCDEngine(senderConfig);

      result.success(null);
    } else if (call.method.equals("startCast")) {
      TCDCastConfig config = new TCDCastConfig();
      config.pin = (String) call.arguments;

      if (delegate.capturePermissionData != null) {
        // startCast directly
        TCDCastPermissionInfo info = new TCDCastPermissionInfo();
        info.permissionCode = delegate.capturePermissionCode;
        info.permissionData = delegate.capturePermissionData;
        sender.startCast(config, info);
      } else {
        // call Delegate
        delegate.current = new Current(
            REQUEST_MEDIA_PROJECTION, result, "cast", sender, config);
        startCast();
      }
    } else if (call.method.equals("stopCast")) {
      sender.stopCast();
      result.success(null);
    } else if (call.method.equals("pauseCast")) {
      // sender.pauseCast();
      result.success(null);
    } else if (call.method.equals("resumeCast")) {
      if (delegate.capturePermissionData != null) {
        TCDRecoveryInfo recovery = new TCDRecoveryInfo();
        // recovery.
        // TODO:

        TCDCastPermissionInfo info = new TCDCastPermissionInfo();
        info.permissionCode = delegate.capturePermissionCode;
        info.permissionData = delegate.capturePermissionData;
        sender.recoveryCast(recovery, info);
      } else {
        // call Delegate
        delegate.current = new Current(
            REQUEST_MEDIA_PROJECTION, result, "recover", sender, null);
        startCast();
      }

      result.success(null);
    } else if (call.method.equals("startNetCheck")) {
      sender.startCheckNetwork();
      result.success(null);
    } else if (call.method.equals("stopNetCheck")) {
      sender.stopCheckNetwork();
      result.success(null);
    } else if (call.method.equals("updateAuth")) {
      sender.updateAuthInfo((String) call.arguments);
      result.success(null);
    } else {
      result.notImplemented();
    }
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

  void startCast() {
    Activity activity = registrar.activity();
    MediaProjectionManager manager =
        (MediaProjectionManager) activity.getSystemService(
            Context.MEDIA_PROJECTION_SERVICE);

    activity.startActivityForResult(
        manager.createScreenCaptureIntent(), REQUEST_MEDIA_PROJECTION);
  }

  @Override
  public void onListen(Object arguments, EventSink events) {
    Log.d(TAG, "onListen " + arguments);
    // chargingStateChangeReceiver =
    // createChargingStateChangeReceiver(events);
    // registrar.context().registerReceiver(chargingStateChangeReceiver,
    //     new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
  }

  @Override
  public void onCancel(Object arguments) {
    Log.d(TAG, "onCancel " + arguments);
    // registrar.context().unregisterReceiver(chargingStateChangeReceiver);
    // chargingStateChangeReceiver = null;
  }

  private final class SenderListener implements TCDSenderListener {
    private final MethodChannel channel;

    SenderListener(MethodChannel channel) {
      this.channel = channel;
    }

    public void onEngineStarted(int errorCode, TCDUser selfInfo) {
      Log.d(TAG,
          "onEngineStarted errorCode: " + errorCode
              + ", selfInfo: " + selfInfo);
      this.channel.invokeMethod("engineStarted", errorCode);
    }

    public void onAuthInfoExpired() {
      channel.invokeMethod("authExpired", null);
    }

    public void onUserChanged(
        int changeType, List<TCDUser> changeList, List<TCDUser> totalList) {
      Log.d(TAG,
          "onUserChanged changeType: " + changeType
              + ", changeList: " + changeList + ", totalList: " + totalList);
      channel.invokeMethod("userChanged", null);
    }

    public void onCastStopped(int reason) {
      Log.d(TAG, "onCastStopped reason: " + reason);
      channel.invokeMethod("castStopped", reason);
    }

    public void onCastStarted(int retCode) {
      Log.d(TAG, "onCastStarted retCode: " + retCode);
      channel.invokeMethod("castStarted", retCode);
    }

    public void onCastAdded(int retCode, TCDCastConfig config) {
      Log.d(TAG, "onCastAdded retCode: " + retCode + ", config: " + config);
      channel.invokeMethod("castAdded", retCode);
    }

    public void onCastStateChanged(int retCode, int castState) {
      Log.d(TAG,
          "onCastStateChanged retCode: " + retCode
              + ", castState: " + castState);
      channel.invokeMethod("castStateChanged", castState);
    }

    public void onRecoveryNotify(TCDRecoveryInfo info) {
      Log.d(TAG, "onRecoveryNotify info: " + info);
      channel.invokeMethod("recover", info.receiverTcdUId);
    }

    public void onRecoveryCompleted(int retCode, TCDRecoveryInfo info) {
      Log.d(TAG, "onRecoveryCompleted retCode: " + retCode + ", info: " + info);
      channel.invokeMethod("recovered", retCode);
    }

    public void onNetStateChanged(boolean disconnected) {
      Log.d(TAG, "onNetStateChanged disconnected: " + disconnected);
      channel.invokeMethod("netStateChanged", disconnected);
    }

    public void onNetworkCheckProgress(
        String url, String description, int progress, int totalSize) {
      Log.d(TAG,
          "onNetworkCheckProgress url: " + url + ", description: " + description
              + ", progress: " + progress + ", totalSize: " + totalSize);

      Map<String, Object> args = new HashMap<>();
      args.put("url", url);
      args.put("description", description);
      args.put("progress", progress);
      args.put("totalSize", totalSize);

      channel.invokeMethod("netCheck", args);
    }

    public void onNetworkCheckFinished(List<PingTask> items) {
      Log.d(TAG, "onNetworkCheckFinished items: " + items);
      channel.invokeMethod("netChecked", null);
    }

    public void onTipsUpdated(String content) {
      if (content.contains("wecast version"))
        return;

      channel.invokeMethod("tips", content);
    }

    public void onKickOut() {
      channel.invokeMethod("kickout", null);
    }

    public void onStreamInfoUpdated() {
      channel.invokeMethod("streaming", null);
    }
  }

  public static final class Current {
    public int request;
    public Result result;
    public String action;
    private TCDEngineSender sender;
    private TCDCastConfig castConfig;

    Current(int request, Result result, String action, TCDEngineSender sender,
        TCDCastConfig castConfig) {
      this.request = request;
      this.result = result;
      this.action = action;
      this.sender = sender;
      this.castConfig = castConfig;
    }
  }

  public static final class Delegate
      implements PluginRegistry.ActivityResultListener {
    private boolean pendingOperation = false;

    public Current current;
    public int capturePermissionCode = 0;
    public Intent capturePermissionData = null;

    @Override
    public boolean onActivityResult(
        int requestCode, int resultCode, Intent data) {
      if (pendingOperation)
        return false;

      Log.d(TAG,
          "onActiveResult request: " + requestCode + " result: " + resultCode
              + " data: " + data);

      if (requestCode == REQUEST_MEDIA_PROJECTION) {
        if (resultCode != Activity.RESULT_OK) {
          if (current != null) {
            current.result.error("ScreenCaptureAccess", "TODO", null);
          }
          return true;
        }

        capturePermissionCode = resultCode;
        capturePermissionData = data;

        if (current != null) {
          current.result.success(resultCode);

          TCDCastPermissionInfo info = new TCDCastPermissionInfo();
          info.permissionCode = capturePermissionCode;
          info.permissionData = capturePermissionData;
          // TODO: recovery
          current.sender.startCast(current.castConfig, info);
        }
        return true;
      }
      return false;
    }
  }
}
