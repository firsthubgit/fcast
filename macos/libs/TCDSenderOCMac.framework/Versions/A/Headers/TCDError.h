//
//  TCDError.h
//  TCDSenderOC
//
//  Created by rogerlin on 2019/9/17.
//  Copyright © 2019 Tencent. All rights reserved.
//

#ifndef TCDError_h
#define TCDError_h

typedef NS_ENUM(NSInteger, TCDError) {
    
    TCDErrorOk     = 0,
    TCDErrorFailed = 1,
    TCDErrorNotInitialized = 2,
    
    // common
    TCDErrorNetworkFailed     = 101,  // 网络异常
    TCDErrorAuthFailed        = 102,  // 授权验证失败
    TCDErrorVersionForbidden  = 103,  // sdk版本被禁用
    TCDErrorParam             = 104,  // 错误参数
    TCDErrorState             = 105,  // 错误状态
    TCDErrorMediaEngineFailed = 106,  // 音视频模块失败
    TCDErrorCmdException      = 107,  // 信令异常
    TCDErrorAbilityNotMatch   = 108,  // 发送端接收端能力不匹配无法互通
    TCDErrorNotSupport        = 109,
    
    TCDErrorCdKeyInvalid = 110,  // cdkey无效
    TCDErrorCdKeyOverdue = 111,  // cdkey欠费
    
    TCDErrorCastTimeout  = 201,  // 建立投屏连接超时
    TCDErrorOperateLimit = 202,  // 操作错误次数过多被限制
    
    TCDErrorInvalidPin = 301,  // 错误pin码
    
    TCDErrorEnd = 999

};

#endif /* TCDError_h */
