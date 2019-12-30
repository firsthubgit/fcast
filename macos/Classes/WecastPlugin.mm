#import "WecastPlugin.h"
#import <Cocoa/Cocoa.h>

// #import "TCDDefine.h"
#import "xcast/xcast.h"

#import "TCDSenderOCMac/TCDDefine.h"

@implementation WecastPlugin {
  TCDEngineSender *_sender;
  FlutterMethodChannel *_channel;
}

- (instancetype)initWithChannel:(FlutterMethodChannel *)channel {
  self = [super init];
  if (self) {
    _channel = channel;
  }
  return self;
}

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar> *)registrar {
  FlutterMethodChannel *channel =
      [FlutterMethodChannel methodChannelWithName:@"wecast"
                                  binaryMessenger:[registrar messenger]];
  WecastPlugin *instance = [[WecastPlugin alloc] initWithChannel:channel];
  [registrar addMethodCallDelegate:instance channel:channel];

  // Debug
  NSAssert(xcast_version(), @"build check");
}

- (void)handleMethodCall:(FlutterMethodCall *)call
                  result:(FlutterResult)result {
  NSLog(@">>> call %@ %@", call.method, call.arguments);
  if ([@"getPlatformVersion" isEqualToString:call.method]) {
    result(
        [@"macOS " stringByAppendingString:[[NSProcessInfo processInfo]
                                               operatingSystemVersionString]]);
  } else if ([@"queryPermission" isEqualToString:call.method]) {
    result(@([self queryPermission]));
  } else if ([@"shutdown" isEqualToString:call.method]) {
    [_sender stopTCDEngine];
    _sender = nil;
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
    config.pin = call.arguments;
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
  } else if ([@"startNetCheck" isEqualToString:call.method]) {
    [_sender startCheckNetwork];
    result(nil);
  } else if ([@"stopNetCheck" isEqualToString:call.method]) {
    [_sender stopCheckNetwork];
    result(nil);
  } else {
    result(FlutterMethodNotImplemented);
  }
}

- (BOOL)queryPermission {
  CGDisplayStreamRef stream = CGDisplayStreamCreate(
      CGMainDisplayID(), 1, 1, kCVPixelFormatType_32BGRA, nil,
      ^(CGDisplayStreamFrameStatus status, uint64_t displayTime,
        IOSurfaceRef frameSurface, CGDisplayStreamUpdateRef updateRef){
      });
  if (stream) {
    CFRelease(stream);
    return YES;
  }
  return NO;

  // copy from
  // https://stackoverflow.com/a/57379206/620672
  // not worked
  if (@available(macOS 10.15, *)) {
    CFArrayRef windowList = CGWindowListCopyWindowInfo(
        kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    NSUInteger numberOfWindows = CFArrayGetCount(windowList);
    NSUInteger numberOfWindowsWithName = 0;
    for (int idx = 0; idx < numberOfWindows; idx++) {
      NSDictionary *windowInfo =
          (NSDictionary *)CFArrayGetValueAtIndex(windowList, idx);
      NSString *windowName = windowInfo[(id)kCGWindowName];
      if (windowName) {
        numberOfWindowsWithName++;
      } else {
        // no kCGWindowName detected -> not enabled
        break;  // breaking early, numberOfWindowsWithName not increased
      }
    }
    CFRelease(windowList);
    return numberOfWindows == numberOfWindowsWithName;
  }
  return YES;
}

- (void)onEngineStarted:(TCDError)code userInfo:(TCDUser *)selfInfo {
  [_channel invokeMethod:@"engineStarted" arguments:@(code)];
}

- (void)onCastStarted:(TCDError)code {
  [_channel invokeMethod:@"castStarted" arguments:@(code)];
}

- (void)onCastStopped:(TCDError)reason {
  [_channel invokeMethod:@"castStopped" arguments:@(reason)];
}

- (void)onUserChanged:(TCDUserChangeType)changeType
           changeList:(NSMutableArray<TCDUser *> *)changeList
            totalList:(NSMutableArray<TCDUser *> *)totalList {
  [_channel invokeMethod:@"userChanged" arguments:nil];
}

- (void)onCastAdded:(TCDError)code config:(TCDCastConfig *)config {
  [_channel invokeMethod:@"castAdded" arguments:@(code)];
}

- (void)onCastStateChanged:(TCDError)code castState:(TCDCastState)castState {
  [_channel invokeMethod:@"castStateChanged" arguments:@(castState)];
}

- (void)onAuthInfoExpired {
  [_channel invokeMethod:@"authExpired" arguments:nil];
}

- (void)onExtendScreenModeChanged:(BOOL)extendMode {
  [_channel invokeMethod:@"extendModeChanged" arguments:@(extendMode)];
}

- (void)onRecoveryNotify:(TCDRecoveryInfo *)info {
  [_channel invokeMethod:@"recover" arguments:info.receiverTCDUID];
}

- (void)onRecoveryComplete:(TCDError)code info:(TCDRecoveryInfo *)info {
  [_channel invokeMethod:@"recovered" arguments:@(code)];
}

- (void)onNetStateChanged:(BOOL)disconnected {
  [_channel invokeMethod:@"netStateChanged" arguments:@(disconnected)];
}

- (void)onNetworkCheckProgress:(NSString *)url
                   description:(NSString *)description
                      progress:(int)progress
                     totalSize:(int)totalSize {
  [_channel invokeMethod:@"netCheck"
               arguments:@{
                 @"description" : description,
                 @"progress" : @(progress),
                 @"total" : @(totalSize),
               }];
}

- (void)onNetworkCheckFinish:(NSMutableArray<TCDPingTask *> *)item {
  [_channel invokeMethod:@"netChecked" arguments:nil];
}

- (void)onTipsUpdate:(NSString *)tips {
  // 这类消息太多了，屏蔽掉
  if ([tips containsString:@"wecast version"])
    return;
  [_channel invokeMethod:@"tip" arguments:tips];
}

- (void)onKickOut {
  [_channel invokeMethod:@"kickout" arguments:nil];
}

- (void)onStreamInfoUpdated {
  [_channel invokeMethod:@"streaming" arguments:nil];
}

@end
