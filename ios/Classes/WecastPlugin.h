#import <Foundation/Foundation.h>

#import "Flutter/FlutterPlugin.h"
#import "TCDSenderOCiOS/TCDSenderOCiOS.h"

@interface WecastPlugin : NSObject<FlutterPlugin, TCDSenderDelegate>
@end
