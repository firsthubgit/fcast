#import <Cocoa/Cocoa.h>
#import "WecastPlugin.h"

// #import "TCDDefine.h"
#import "xcast/xcast.h"

#import "TCDSenderOCMac/TCDDefine.h"

@implementation WecastPlugin

TCDEngineSender *_sender;

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  FlutterMethodChannel* channel = [FlutterMethodChannel
      methodChannelWithName:@"wecast"
            binaryMessenger:[registrar messenger]];
  WecastPlugin* instance = [[WecastPlugin alloc] init];
  [registrar addMethodCallDelegate:instance channel:channel];
    
  // Debug
  NSLog(@"cast version: %s", xcast_version());
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
  if ([@"getPlatformVersion" isEqualToString:call.method]) {
    result([@"macOS " stringByAppendingString:[[NSProcessInfo processInfo] operatingSystemVersionString]]);
  } else if ([@"init" isEqualToString:call.method]) {
      _sender = [[TCDEngineSender alloc] init];
      [_sender initTCDEngineSender];
      [_sender setSenderDelegate:self];

      TCDAbilityConfig *abilityConfig = [[TCDAbilityConfig alloc] init];
      abilityConfig.useAudioCapture = [call.arguments[@"captureAudio"] boolValue];
      abilityConfig.useExtendScreen = [call.arguments[@"extendScreen"] boolValue];
      [_sender setAbilityConfig:abilityConfig];

      TCDSenderConfig *senderConfig = [[TCDSenderConfig alloc] init];
      senderConfig.corpId = call.arguments[@"corpId"];
      senderConfig.corpAuth = call.arguments[@"corpAuth"];
      senderConfig.nickName = call.arguments[@"nickName"];

      TCDPrivateConfig *privateConfig = [[TCDPrivateConfig alloc] init];
      [privateConfig setPrivateUrl:call.arguments[@"privateUrl"]];
      [_sender setPrivateConfig:privateConfig];

      [_sender startTCDEngine:senderConfig];
      // result(@true);
      result(nil);
  } else if ([@"startCast" isEqualToString:call.method]) {
      TCDCastConfig *config = [[TCDCastConfig alloc] init];
      config.pin = call.arguments[@"pin"];;
      [_sender startCast:config];
      result(nil);
  } else if ([@"stopCast" isEqualToString:call.method]) {
      [_sender stopCast];
      result(nil);
  } else if ([@"pauseCast" isEqualToString:call.method]) {
      [_sender pauseCast:true];
      result(nil);
  } else if ([@"resumeCast" isEqualToString:call.method]) {
     [_sender pauseCast:false];
     result(nil);
  } else {
    result(FlutterMethodNotImplemented);
  }
}


- (void)onEngineStarted:(TCDError)code userInfo:(TCDUser *)selfInfo {}

- (void)onCastStarted:(TCDError)code {}

- (void)onCastStopped:(TCDError)reason {}

- (void)onUserChanged:(TCDUserChangeType)changeType
           changeList:(NSMutableArray<TCDUser *> *)changeList
            totalList:(NSMutableArray<TCDUser *> *)totalList {}

- (void)onCastAdded:(TCDError)code config:(TCDCastConfig *)config {}

- (void)onCastStateChanged:(TCDError)code castState:(TCDCastState)castState {}

- (void)onAuthInfoExpired {}


- (void)onExtendScreenModeChanged:(BOOL)extendMode {}

- (void)onNetStateChanged:(BOOL)disconnected {}

- (void)onRecoveryNotify:(TCDRecoveryInfo *)info {}

- (void)onRecoveryComplete:(TCDError)code info:(TCDRecoveryInfo *)info {}

- (void)onNetworkCheckProgress:(NSString *)url
                   description:(NSString *)description
                      progress:(int)progress
                     totalSize:(int)totalSize {}

- (void)onNetworkCheckFinish:(NSMutableArray<TCDPingTask *> *)item {}

- (void)onTipsUpdate:(NSString *)tips {}

- (void)onKickOut {}

- (void)onStreamInfoUpdated {}


@end
