import 'dart:async';

import 'package:flutter/services.dart';

class Setting {
  Setting({
    this.captureAudio: false,
    this.extendScreen: true,
    this.mirror: false,
    this.corpId,
    this.corpAuth,
    this.nickName,
    this.privateUrl,
  })  : assert(corpId != null),
        assert(corpAuth != null),
        assert(nickName != null),
        assert(privateUrl != null);

  final bool captureAudio;
  final bool extendScreen;
  final bool mirror;

  final String corpId;
  final String corpAuth;
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

typedef ErrorHandle = void Function(String);
typedef StateChange = void Function(CastState);

class Wecast {
  static Future<String> get platformVersion async {
    if (_instance == null) return null;

    return await _instance._channel.invokeMethod('getPlatformVersion');
  }

  static Wecast _instance;

  static Future<Wecast> init(
    Setting setting,
    ErrorHandle errorHandle,
    StateChange stateChange,
  ) async {
    assert(_instance == null);
    _instance = Wecast._(errorHandle, stateChange);

    await _instance._channel.invokeMethod<bool>('init', {
      'extendScreen': setting.extendScreen,
      'captureAudio': setting.captureAudio,
      'corpId': setting.corpId,
      'corpAuth': setting.corpAuth,
      'privateUrl': setting.privateUrl,
      'nickName': setting.nickName,
    });
    return _instance;
  }

  Wecast._(this.errorHandle, this.stateChange) {
    _channel.setMethodCallHandler(_callback);
  }

  final MethodChannel _channel = const MethodChannel('wecast');
  final ErrorHandle errorHandle;
  final StateChange stateChange;

  CastState _state = CastState.stateNone;

  CastState get state => _state;

  Future<void> startCast(String pin) async {
    return await _channel.invokeMethod<bool>('startCast', pin);
  }

  Future<void> stopCast() async {
    return await _channel.invokeMethod<bool>('stopCast');
  }

  Future<void> pauseCast() async {
    return await _channel.invokeMethod<bool>('pauseCast');
  }

  Future<void> resumeCast() async {
    return await _channel.invokeMethod<bool>('resumeCast');
  }

  ///
  Future<void> startNetCheck() async {
    return await _channel.invokeMethod<bool>('startNetCheck');
  }

  Future<Null> _callback(MethodCall method) async {
    final List normalCoded = <String>[
      'engineStarted',
    ];

    print('>>> callback: ${method.method} ${method.arguments} '
        '${method.arguments.runtimeType} '
        '${errorHandle != null && normalCoded.contains(method.method)} '
        '$errorHandle');

    if (errorHandle != null && normalCoded.contains(method.method)) {
      errorHandle(error(method.arguments));
    }

    // if (method.method == 'castStarted')
    //   _state = CastState.stateCasting;
    // else if (method.method == 'castStopped')
    //   _state = CastState.stateNone;
    // else
    if (method.method == 'castStateChanged') {
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
      301: '错误pin码'
    };

    if (table.containsKey(code)) {
      return "${table[code]}($code)";
    }
    return '未知错误($code)';
  }
}
