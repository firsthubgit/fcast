/*
** Copyright (c) 2018 The WeCast project. All rights reserved.
*/

#ifndef WMP_TCD_ENGINE_DEF_H_
#define WMP_TCD_ENGINE_DEF_H_

// #if defined(WIN32) || defined(_WIN32)
//   #ifdef TCDEXPORT
//     #define TCD_API __declspec(dllexport)
//   #else
//     #ifdef TCDSTATIC
//       #define TCD_API
//     #else
//       #define TCD_API __declspec(dllimport)
//     #endif
//   #endif
// #else
  #define TCD_API __declspec(dllimport)
// #endif

// class tcd::TCDEngineSender * tcd::GetTCDSenderEngine(void)
#include <stdint.h>

namespace tcd {

enum TCDError {
  kTCDErrorOk     = 0,
  kTCDErrorFailed = 1,

  // common
  kTCDErrorNetworkFailed     = 101,  // 网络异常
  kTCDErrorAuthFailed        = 102,  // 授权验证失败
  kTCDErrorVersionForbidden  = 103,  // sdk版本被禁用
  kTCDErrorParam             = 104,  // 错误参数
  kTCDErrorState             = 105,  // 错误状态
  kTCDErrorMediaEngineFailed = 106,  // 音视频模块失败
  kTCDErrorCmdException      = 107,  // 信令异常
  kTCDErrorAbilityNotMatch   = 108,  // 发送端接收端能力不匹配无法互通
  kTCDErrorNotSupport        = 109,  // 不支持的功能

  kTCDErrorCdKeyInvalid = 110,  // cdkey无效
  kTCDErrorCdKeyOverdue = 111,  // cdkey欠费

  kTCDErrorCastTimeout  = 201,  // 建立投屏连接超时
  kTCDErrorOperateLimit = 202,  // 操作错误次数过多被限制

  kTCDErrorInvalidPin = 301,  // 错误pin码

  kTCDErrorDeviceNotSupport = 401, // 不支持多于两个显示器的扩展屏,投屏过程中热插拔也会导致扩展屏功能不可用并返回该错误。

  kTCDErrorEnd = 999
};

enum TCDRole {
  /// <summary>
  /// 发送端
  /// </summary>
  kTCDRoleSender = 0,
  /// <summary>
  /// 接收端
  /// </summary>
  kTCDRoleReceiver = 1
};

enum TCDPlatform {
  /// <summary>
  /// 空
  /// </summary>
  kTCDPlatformNone = 0,
  /// <summary>
  /// Windows
  /// </summary>
  kTCDPlatformWindows = 1,
  /// <summary>
  /// macOS
  /// </summary>
  kTCDPlatformMac = 2,
  /// <summary>
  /// iOS
  /// </summary>
  kTCDPlatformIOS = 3,
  /// <summary>
  /// Android
  /// </summary>
  kTCDPlatformAndroid = 4
};

enum UserChangeType {
  /// <summary>
  /// 用户进入投屏房间
  /// </summary>
  kUserChangeTypeEnter = 1,
  /// <summary>
  /// 用户退出投屏房间
  /// </summary>
  kUserChangeTypeOut = 2,
  /// <summary>
  /// 用户投屏暂停
  /// </summary>
  kUserChangeTypePause = 3,
  /// <summary>
  /// 用户投屏暂停后恢复
  /// </summary>
  kUserChangeTypeResume = 4,
  /// <summary>
  /// 用户状态信息变化
  /// </summary>
  KUserChangeTypeUpdate = 5
};

struct TCDChannelConfig {
  /// <summary>
  /// 定制化渠道需要填此信息
  /// </summary>
  int32_t channel_id = 1;
};

struct TCDPrivateConfig {
  /// <summary>
  /// 定制化渠道config 服务域名
  /// </summary>
  const char* url = nullptr;
};

struct TCDUser {
  /// <summary>
  /// 企业ID
  /// </summary>
  const char* corp_id = nullptr;
  /// <summary>
  /// 用户昵称
  /// </summary>
  const char* nickname = nullptr;
  /// <summary>
  /// 投屏用户id
  /// </summary>
  const char* tcd_uid = nullptr;
  /// <summary>
  /// 用户角色，kTCDRoleSender/kTCDRoleReceiver
  /// </summary>
  TCDRole role = kTCDRoleSender;
  /// <summary>
  /// 平台
  /// </summary>
  TCDPlatform platform = kTCDPlatformNone;
  /// <summary>
  /// 推流服务的发现信息（普通投屏无需关注）
  /// </summary>
  const char* discovery_info = nullptr;
};

struct TCDAbilityConfig {
    /// <summary>
    /// 开启投送画面时带上系统播放声音的能力
    /// </summary>
    bool using_audio_capture = true;

    /// <summary>
    /// 开启扩展屏分享的能力
    /// </summary>
    bool using_extend_screen = true;

    /// <summary>
    /// 开启镜像的能力
    /// </summary>
    bool using_mirror = true;
};

struct PingTask {
  /// <summary>
  /// 网络检测域名
  /// </summary>
  const char* url;
  /// <summary>
  /// 检测域名的描述
  /// </summary>
  const char* description;
  /// <summary>
  /// 检测成功次数
  /// </summary>
  int32_t success_times;
  /// <summary>
  /// 检测总次数
  /// </summary>
  int32_t total_ping_times;

  PingTask(const char* i_url, const char* i_description, int32_t i_success_time, int32_t i_total_ping_time) {
    url              = i_url;
    description      = i_description;
    success_times    = i_success_time;
    total_ping_times = i_total_ping_time;
  }
};

enum TCDEngineState {
  kTCDEngineStateNone = 0,
  kTCDEngineStateInited,    // 启动过但启动失败了的状态
  kTCDEngineStateStarting,  // 启动中
  kTCDEngineStateStarted    // 启动完成
};

enum TCDEngineCastState {
  kTCDEngineCastNone = 0,
  kTCDCastOperating,    // 暂停操作进行中（触发了暂停但还未请求完成）
  kTCDEngineCasting,    // 投屏中
  kTCDEngineCastPaused  // 投屏已暂停
};

}  // namespace tcd

#endif  // WMP_TCD_ENGINE_DEF_H_
