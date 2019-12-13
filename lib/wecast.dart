import 'dart:async';

import 'package:flutter/services.dart';

class Wecast {
  static const MethodChannel _channel =
      const MethodChannel('wecast');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
