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

class Wecast {
  static const MethodChannel _channel = const MethodChannel('wecast');

  static Future<String> get platformVersion async {
    return await _channel.invokeMethod('getPlatformVersion');
  }

  static Future<Wecast> init(Setting setting) async {
    await _channel.invokeMethod<bool>('init', {
      'extendScreen': setting.extendScreen,
      'captureAudio': setting.captureAudio,
      'corpId': setting.corpId,
      'corpAuth': setting.corpAuth,
      'privateUrl': setting.privateUrl,
      'nickName': setting.nickName,
    });
    return Wecast();
  }

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
}
