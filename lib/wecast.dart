import 'dart:async';

import 'package:flutter/services.dart';

import 'rsa_encrypt.dart';

class Setting {
  Setting({
    this.publicKey,
    this.captureAudio: false,
    this.extendScreen: true,
    this.mirror: false,
    this.corpId,
    this.nickName,
    this.privateUrl,
  })  : assert(publicKey != null),
        assert(corpId != null),
        assert(nickName != null),
        assert(privateUrl != null),
        assert(extendScreen != mirror);

  final String publicKey;
  final bool captureAudio;
  final bool extendScreen;
  final bool mirror;
  final String corpId;
  final String nickName;
  final String privateUrl;
}

enum CastState {
  stateNone,

  /// 暂停操作进行中（触发了暂停但还未请求完成）
  statePauseOperating,
  stateCasting,
  statePaused,
}

typedef ErrorHandle = void Function(int, String);
typedef StateChange = void Function(CastState);

class Wecast {
  static Future<String> get platformVersion async {
    return await _channel.invokeMethod('getPlatformVersion');
  }

  static Future<bool> queryPermission() async {
    return await _channel.invokeMethod<bool>('queryPermission');
  }

  static const MethodChannel _channel = const MethodChannel('wecast');
  static Wecast _instance;

  static Future<Wecast> init(
    Setting setting,
    ErrorHandle errorHandle,
    StateChange stateChange,
  ) async {
    assert(_instance == null);
    if (_instance == null) {
      _instance = Wecast._(errorHandle, stateChange);

      _channel.invokeMethod<void>('init', {
        'extendScreen': setting.extendScreen,
        'captureAudio': setting.captureAudio,
        'corpId': setting.corpId,
        'corpAuth': genCorpAuth(setting),
        'privateUrl': setting.privateUrl,
        'nickName': setting.nickName,
        'mirror': setting.mirror,
      });
    }
    return _instance;
  }

  static String genCorpAuth(Setting setting) {
    return rsaEncryptWith(setting.publicKey, {
      'corpid': setting.corpId,
      'timestamp': '${DateTime.now().millisecondsSinceEpoch / 1000}',
    });
  }

  Wecast._(this.errorHandle, this.stateChange) {
    _channel.setMethodCallHandler(_callback);
  }

  final ErrorHandle errorHandle;
  final StateChange stateChange;

  CastState _state = CastState.stateNone;
  CastState get state => _state;

  Future startCast(String pin) async {
    await _channel.invokeMethod<void>('startCast', pin);
  }

  Future stopCast() async {
    await _channel.invokeMethod<void>('stopCast');
  }

  Future pauseCast() async {
    await _channel.invokeMethod<void>('pauseCast');
  }

  Future resumeCast() async {
    await _channel.invokeMethod<void>('resumeCast');
  }

  ///
  Future startNetCheck() async {
    await _channel.invokeMethod<void>('startNetCheck');
  }

  Future shutdown() async {
    await _channel.invokeMethod<void>('shutdown');
    _channel.setMethodCallHandler(null);
  }

  Future _callback(MethodCall method) async {
    // 回调参数为错误码的函数
    final List codedMethod = <String>[
      'engineStarted',
      'castStopped',
      'castAdded',
      'recovered',
      'castStarted',
    ];

    print('>>> callback: ${method.method} ${method.arguments} '
        '${method.arguments.runtimeType} '
        '${errorHandle != null && codedMethod.contains(method.method)}');

    if (errorHandle != null && codedMethod.contains(method.method)) {
      errorHandle(method.arguments, error(method.arguments));
    }

    final List stateMethod = <String>['castStateChanged'];

    // if (method.method == 'castStarted')
    //   _state = CastState.stateCasting;
    // else if (method.method == 'castStopped')
    //   _state = CastState.stateNone;
    // else
    if (stateMethod.contains(method.method)) {
      _state = method.arguments;
      stateChange(_state);
    }
  }

  static String error(int code) {
    final Map<int, String> table = {
      0: '成功',
      1: '失败',
      2: '未初始化',
      101: '网络异常',
      102: '授权验证失败',
      103: 'sdk版本被禁用',
      104: '错误参数',
      105: '错误状态',
      106: '音视频模块失败',
      107: '信令异常',
      108: '发送端接收端能力不匹配无法互通',
      110: 'cdkey无效',
      111: 'cdkey欠费',
      201: '建立投屏连接超时',
      202: '操作错误次数过多被限制',
      301: '错误pin码',
      80009: '心跳超时', // kTCDExitTimeout
      80010: '邀请失败或超时退出', // kTCDExitByInviteFailed
      80017: 'tv主动断开', // kTCDTvExit
      80018: 'tv邀请超时(5s)', // kTCDTVNotEnter
      80021: '合盖子退出', // kTCDBookCover
      80022: 'app被终止', // kTCDTerminateApp
      80023: '视频流传输超时(10s)', // kTCDXcastStreamTimeout
      80024: 'iOS锁屏', // kTCDiOSScreenLock
      80025: 'iOS ReplayKit 意外停止', // kTCDReplayKitStop
      80031: 'Windows 锁屏', // kTCDWinLock
      80033: '无首帧，倒计时退出', // kTCDNoFirstFrameExit
      80052: '重连后发现对端断开', // kTCDUserChangeAfterReconnect
      80054: '被抢投退出', // kTCDExitByGrabbed
    };

    if (table.containsKey(code)) {
      return "${table[code]}($code)";
    }
    return '未知错误($code)';
  }
}
