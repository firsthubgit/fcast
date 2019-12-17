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
  } else {
    result(FlutterMethodNotImplemented);
  }
}

- (BOOL)queryPermission {
  BOOL canRecordScreen = YES;
  if (@available(macOS 10.15, *)) {
    canRecordScreen = NO;
    NSRunningApplication *runningApplication = NSRunningApplication.currentApplication;
    NSNumber *ourProcessIdentifier = [NSNumber numberWithInteger:runningApplication.processIdentifier];

    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    NSUInteger numberOfWindows = CFArrayGetCount(windowList);
    for (int index = 0; index < numberOfWindows; index++) {
        // get information for each window
        NSDictionary *windowInfo = (NSDictionary *)CFArrayGetValueAtIndex(windowList, index);
        NSString *windowName = windowInfo[(id)kCGWindowName];
        NSNumber *processIdentifier = windowInfo[(id)kCGWindowOwnerPID];

        // don't check windows owned by this process
        if (![processIdentifier isEqual:ourProcessIdentifier]) {
            // get process information for each window
            pid_t pid = processIdentifier.intValue;
            NSRunningApplication *windowRunningApplication = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
            if (!windowRunningApplication) {
                // ignore processes we don't have access to, such as WindowServer, which manages the windows named "Menubar" and "Backstop Menubar"
            }
            else {
                NSString *windowExecutableName = windowRunningApplication.executableURL.lastPathComponent;
                if (windowName) {
                    if ([windowExecutableName isEqual:@"Dock"]) {
                        // ignore the Dock, which provides the desktop picture
                    }
                    else {
                        canRecordScreen = YES;
                        break;
                    }
                }
            }
        }
    }
    CFRelease(windowList);
  }
  return canRecordScreen;
}


- (void)onEngineStarted:(TCDError)code userInfo:(TCDUser *)selfInfo {
  [_channel invokeMethod:@"engineStarted"
               arguments:@(code)];
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

- (void)onNetStateChanged:(BOOL)disconnected {
  [_channel invokeMethod:@"netStateChanged" arguments:@(disconnected)];
}

- (void)onRecoveryNotify:(TCDRecoveryInfo *)info {
  [_channel invokeMethod:@"recover" arguments:info.receiverTCDUID];
}

- (void)onRecoveryComplete:(TCDError)code info:(TCDRecoveryInfo *)info {
  [_channel invokeMethod:@"recovered" arguments:@(code)];
}

- (void)onNetworkCheckProgress:(NSString *)url
                   description:(NSString *)description
                      progress:(int)progress
                     totalSize:(int)totalSize {
  [_channel invokeMethod:@"netCheck" arguments:description];
}

- (void)onNetworkCheckFinish:(NSMutableArray<TCDPingTask *> *)item {
  [_channel invokeMethod:@"netChecked" arguments:nil];
}

- (void)onTipsUpdate:(NSString *)tips {
  [_channel invokeMethod:@"tip" arguments:tips];
}

- (void)onKickOut {
  [_channel invokeMethod:@"kickout" arguments:nil];
}

- (void)onStreamInfoUpdated {
  [_channel invokeMethod:@"stream" arguments:nil];
}

@end
