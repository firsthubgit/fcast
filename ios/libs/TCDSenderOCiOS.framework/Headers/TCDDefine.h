//
//  TCDDefine.h
//  TCDSenderOC
//
//  Created by rogerlin on 2019/9/16.
//  Copyright © 2019 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TCDError.h"
NS_ASSUME_NONNULL_BEGIN

#ifndef TCDDefine_h
#define TCDDefine_h

@protocol TCDSenderDelegate;

typedef NS_ENUM(NSInteger, TCDRole)
{
    TCDRoleSender     = 0, //投屏发送端
    TCDRoleReceiver   = 1, //投屏接收端
};

typedef NS_ENUM(NSInteger, TCDAbility)
{
    TCDAbilityAudioCapture   = 1, //投屏投声音能力
    TCDAbilityExtendScreen   = 2, //投屏扩展屏
};


typedef NS_ENUM(NSInteger, TCDUserChangeType)
{
    TCDUserChangeTypeEnter  = 0, // 用户进入投屏房间
    TCDUserChangeTypeOut    = 1, // 用户退出投屏房间
    TCDUserChangeTypePause  = 2, // 用户投屏暂停
    TCDUserChangeTypeResume = 3, // 用户投屏暂停后恢复
    TCDUserChangeTypeUpdate = 4  // 用户状态信息变化
};

typedef NS_ENUM(NSInteger, TCDCastState)
{
    TCDCastStateNone           = 0, //default
    TCDCastStatePauseOperating = 1, //暂停操作进行中（触发了暂停但还未请求完成）
    TCDCastStateCasting        = 2, //投屏中
    TCDCastStatePaused         = 3  //投屏已暂停
};

@interface TCDUser : NSObject

/*!
 @abstract      投屏用户的企业Id
 */
@property (nonatomic, copy) NSString *corpId;

/*!
 @abstract      投屏用户的名字
 */
@property (nonatomic, copy) NSString *nickName;

/*!
 @abstract      投屏用户的唯一ID
 */
@property (nonatomic, copy) NSString *TCDUID;

/*!
 @abstract      投屏角色，iOS和macOS都为发送端
 */
@property (nonatomic, assign) TCDRole role;

/*!
 @abstract      推流服务的发现信息（普通投屏无需关注）
 */
@property (nonatomic, copy) NSString *discoveryInfo;

@end


@interface TCDRecoveryInfo : NSObject

/*!
 @abstract      重新连接的投屏会议室唯一id
 */
@property (nonatomic, copy) NSString *receiverTCDUID;

/*!
 @abstract      重新连接的投屏会议室标识
 */
@property (nonatomic, copy) NSString *receiverNickName;

@property (nonatomic, assign) NSInteger lastExitTime;

@property (nonatomic, assign) BOOL receiverInRoom;

@property (nonatomic, assign) BOOL cancel;

@end


@interface TCDSenderConfig : NSObject

@property (nonatomic, copy) NSString *corpId;

@property (nonatomic, copy) NSString *nickName;

@property (nonatomic, copy) NSString *corpAuth;

@end

@interface TCDChannelConfig : NSObject

@property (nonatomic, assign) int channelId; /// 渠道id 1-腾讯云普通渠道，其他渠道暂时不支持，默认为1
@end

@interface TCDPrivateConfig : NSObject

@property (nonatomic, copy) NSString *privateUrl; /// 私有化url
@end

@interface TCDAbilityConfig : NSObject

@property (nonatomic, assign) BOOL useAudioCapture; /// 投送画面时带上系统播放声音的能力

@property (nonatomic, assign) BOOL useExtendScreen; /// 扩展屏分享的能力

@property (nonatomic, assign) BOOL useMirror;     /// 是否使用镜像
@end

@interface TCDCastConfig : NSObject

@property (nonatomic, copy) NSString *pin;

@end

@interface TCDPingTask : NSObject

@property (nonatomic, copy) NSString *url; /// 网络检测域名

@property (nonatomic, copy) NSString *urlDescription; /// 检测域名的描述

@property (nonatomic, assign) int successTimes; /// 检测成功次数

@property (nonatomic, assign) int totalPingTimes; /// 检测总次数

@end

@protocol TCDSenderDelegate <NSObject>

- (void)onEngineStarted:(TCDError)code userInfo:(TCDUser *)selfInfo;

- (void)onCastStarted:(TCDError)code;

- (void)onCastStopped:(TCDError)reason;

- (void)onUserChanged:(TCDUserChangeType)changeType
           changeList:(NSMutableArray<TCDUser *> *)changeList
            totalList:(NSMutableArray<TCDUser *> *)totalList;

- (void)onCastAdded:(TCDError)code config:(TCDCastConfig *)config;

- (void)onCastStateChanged:(TCDError)code castState:(TCDCastState)castState;

- (void)onAuthInfoExpired;

#pragma mark - optional
- (void)onExtendScreenModeChanged:(BOOL)extendMode;

- (void)onNetStateChanged:(BOOL)disconnected;

- (void)onRecoveryNotify:(TCDRecoveryInfo *)info;

- (void)onRecoveryComplete:(TCDError)code info:(TCDRecoveryInfo *)info;

- (void)onNetworkCheckProgress:(NSString *)url
                   description:(NSString *)description
                      progress:(int)progress
                     totalSize:(int)totalSize;

- (void)onNetworkCheckFinish:(NSMutableArray<TCDPingTask *> *)item;

- (void)onTipsUpdate:(NSString *)tips;

- (void)onKickOut;

- (void)onStreamInfoUpdated;

@end

#endif /* TCDDefine_h */

NS_ASSUME_NONNULL_END
