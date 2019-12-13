#import <Cocoa/Cocoa.h>
#import "WecastPlugin.h"

// #import "TCDDefine.h"
#import "xcast/xcast.h"

#import "TCDSenderOCMac/TCDDefine.h"
#import "TCDSenderOCMac/TCDEngineSender.h"

@implementation WecastPlugin
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
  } else {
    result(FlutterMethodNotImplemented);
  }
}

@end
