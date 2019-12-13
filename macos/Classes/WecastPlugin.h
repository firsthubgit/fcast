#import <Foundation/Foundation.h>
#import <FlutterMacOS/FlutterMacOS.h>

#import "TCDSenderOCMac/TCDEngineSender.h"

@interface WecastPlugin : NSObject<FlutterPlugin, TCDSenderDelegate>
@end
