/*
** Copyright (c) 2018 The WeCast project. All rights reserved.
*/

#ifndef WMP_TCD_ENGINE_COMMON_H_
#define WMP_TCD_ENGINE_COMMON_H_

#include "tcd/tcd_engine_def.h"

namespace tcd {
/// <summary>
/// 设置自定义log存放目录路径
/// </summary>
TCD_API void        SetLogDir(const char* log_dir);

/// <summary>
/// 获取最后一次出错时的错误信息
/// </summary>
TCD_API const char* GetErrorMsg();

/// <summary>
/// 获取SDK版本号
/// </summary>
/// <returns></returns>
TCD_API const char* GetSDKVersion();

class TCDListener {
public:
  virtual ~TCDListener() = default;

  /// <summary>
  /// SDK启动的回调
  /// </summary>
  /// <param name="error_code">启动成功或失败的错误码（成功时错误码为0，参见TCDError）</param>
  /// <param name="self_info">接收端设备自身的信息</param>
  virtual void OnEngineStarted(int32_t error_code, TCDUser* self_info) = 0;

  /// <summary>
  /// 企业授权码失效的回调（需要出发调用updateAuthInfo接口）
  /// </summary>
  virtual void OnAuthInfoExpired() = 0;

  /// <summary>
  /// 投屏中房间内成员信息变化的回调
  /// </summary>
  /// <param name="change_type">成员信息变化类型，参见UserChangeType</param>
  /// <param name="change_list">变化用户列表</param>
  /// <param name="change_size">变化用户列表大小</param>
  /// <param name="total_list">全部用户列表</param>
  /// <param name="total_size">全部用户列表大小</param>
  virtual void OnUserChanged(
      UserChangeType change_type, const TCDUser* change_list, int32_t change_size, const TCDUser* total_list, int32_t total_size) {}

  /// <summary>
  /// 网络状态发生变化的回调
  /// </summary>
  /// <param name="disconnected">true-表示网络已断开</param>
  virtual void OnNetStateChanged(bool disconnected) {}

  /// <summary>
  /// 投屏退出的回调
  /// </summary>
  /// <param name="reason">造成投屏退出的原因 0-表示正常退出</param>
  virtual void OnCastStopped(int32_t reason) {}

  /// <summary>
  /// 流媒体相关信息内容更新的回调
  /// </summary>
  /// <param name="content">流媒体信息</param>
  virtual void OnTipsUpdated(const char* content) {}

  /// <summary>
  /// 网络检测进度的回调
  /// </summary>
  /// <param name="task">当前检测的任务信息</param>
  /// <param name="progress">当前检测进度</param>
  /// <param name="total_size">全部任务大小</param>
  virtual void OnNetworkCheckProgress(const PingTask& task, int32_t progress, int32_t total_size) {}

  /// <summary>
  /// 网络检测已完成的回调
  /// </summary>
  /// <param name="ping_list">每个检测的任务的结果信息列表</param>
  /// <param name="ping_list_size">列表长度</param>
  virtual void OnNetworkCheckFinished(const PingTask* ping_list, int32_t ping_list_size) {}

  /// <summary>
  /// 用户在新的设备登录，当前设备被踢下线，此时sdk已经stop，需要重新startup 才能使用。
  /// </summary>
  virtual void OnKickOut() {}
};
}  // namespace tcd

#endif  // WMP_TCD_ENGINE_COMMON_H_
