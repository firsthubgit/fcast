#import "AppDelegate.h"
#import "GeneratedPluginRegistrant.h"

#import "WecastPlugin.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  [GeneratedPluginRegistrant registerWithRegistry:self];
  // Override point for customization after application launch.
  [WecastPlugin registerWithRegistrar:[self registrarForPlugin:@"wecast"]];
  return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

@end
