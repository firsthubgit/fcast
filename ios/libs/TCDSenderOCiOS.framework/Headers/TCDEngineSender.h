//
//  TCDEngineSender.h
//  TCDSenderOC
//
//  Created by rogerlin on 2019/9/16.
//  Copyright © 2019 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TCDDefine.h"

NS_ASSUME_NONNULL_BEGIN

@interface TCDEngineSender : NSObject

- (void)initTCDEngineSender;
- (NSString *)getSDKVersion;
- (BOOL)startStreamService;
- (void)stopStreamService;
- (void)updateStreamService;
- (void)setSenderDelegate:(id<TCDSenderDelegate>)delegate;
- (void)setAppChannel:(TCDChannelConfig *)channel;
- (void)setPrivateConfig:(TCDPrivateConfig *)channel;
- (void)setAbilityConfig:(TCDAbilityConfig *)config;
- (void)checkAbility:(TCDAbility)ability;
- (void)disableLogReport;
- (void)startTCDEngine:(TCDSenderConfig *)config;
- (void)stopTCDEngine;
- (void)startCast:(TCDCastConfig *)config;
- (void)stopCast;
- (void)addCast:(TCDCastConfig *)config;
- (void)pauseCast:(BOOL)isPause;
- (TCDCastState)getCastState;
- (TCDError)enableAudio:(BOOL)enable;
- (TCDError)enableExtendScreen:(BOOL)enable;
- (void)recoveryCast:(TCDRecoveryInfo *)info;
- (void)updateAuthInfo:(NSString *)corpAuth;
- (void)startCheckNetwork;
- (void)stopCheckNetwork;


@end

NS_ASSUME_NONNULL_END
