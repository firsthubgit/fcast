/*
** Copyright (c) 2018 The WeCast project. All rights reserved.
*/

#ifndef TCD_TCD_ENGINE_SENDER_H_
#define TCD_TCD_ENGINE_SENDER_H_

#include "tcd/tcd_engine_common.h"

namespace tcd {

enum TCDAbility { 
  /// <summary>
  /// 声音采集能力
  /// </summary>
  kTCDAbilityAudioCapture = 1,

  /// <summary>
  /// 扩展屏能力
  /// </summary>
  kTCDAbilityExtendScreen,

  /// <summary>
  /// 镜像能力
  /// </summary>
  kTCDAbilityEnableXcast
};

struct TCDSenderConfig {
  /// <summary>
  /// 企业id
  /// </summary>
  const char*  corp_id   = nullptr;

  /// <summary>
  /// 昵称
  /// </summary>
  const char*  nickname  = nullptr;

  /// <summary>
  /// 企业鉴权
  /// </summary>
  const char*  corp_auth = nullptr;
};

struct TCDRecoveryInfo {
  /// <summary>
  /// 上次断开连接的接收端id
  /// </summary>
  const char* receiver_wmp_uid  = nullptr;

  /// <summary>
  /// 上次断开连接的接收端昵称
  /// </summary>
  const char* receiver_nickname = nullptr;

  /// <summary>
  /// 上次断开连接的时间戳
  /// </summary>
  int64_t     last_exit_time    = 0;

  /// <summary>
  /// 接收端是否还在房间中
  /// </summary>
  bool        receiver_in_room  = true;

  /// <summary>
  /// 取消恢复，此标志位置true.
  /// </summary>
  bool        cancel            = false;
};

struct TCDCastConfig {
  /// <summary>
  /// pin码
  /// </summary>
  const char* pin    = nullptr;
};

class TCDSenderListener : public TCDListener {
public:
  virtual ~TCDSenderListener() = default;

  /// <summary>
  /// engine启动完成回调
  /// </summary>
  /// <param name="ret_code">详见错误码定义</param>
  virtual void OnCastStarted(int ret_code) = 0;

  /// <summary>
  /// 添加接收端完成回调
  /// </summary>
  /// <param name="ret_code">详见错误码定义</param>
  /// <param name="config">对应添加时传入的config</param>
  virtual void OnCastAdded(int ret_code, const TCDCastConfig& config) = 0;

  /// <summary>
  /// 投屏状态变更回调，暂停/恢复投屏时触发
  /// </summary>
  /// <param name="ret_code">详见错误码定义</param>
  /// <param name="cast_state">详见枚举值定义</param>
  virtual void OnCastStateChanged(int ret_code, TCDEngineCastState cast_state) {}

  /// <summary>
  /// 检测到上一次投屏意外断开的回调通知
  /// </summary>
  /// <param name="info">调用恢复投屏是，传入入此信息，详见TCDRecoveryInfo定义</param>
  virtual void OnRecoveryNotify(const TCDRecoveryInfo& info) {}

  /// <summary>
  /// 恢复完成回调
  /// </summary>
  /// <param name="ret_code">详见错误码定义</param>
  /// <param name="info">对应调用传入的info,详见TCDRecoveryInfo定义</param>
  virtual void OnRecoveryCompleted(int ret_code, const TCDRecoveryInfo& info) {}

  /// <summary>
  /// 扩展屏状态变更回调
  /// </summary>
  /// <param name="extend_mode">扩展状态 true 为处于扩展屏状态， false反之</param>
  virtual void OnExtendScreenModeChanged(bool extend_mode) {}
};

class TCDEngineSender {
protected:
  virtual ~TCDEngineSender() = default;

public:

  /// <summary>
  /// 设置回调监听对象
  /// </summary>
  /// <param name="listener">回调监听对象</param>
  virtual void SetListener(TCDSenderListener* listener) = 0;

  /// <summary>
  /// 设置App渠道，默认都是腾讯云渠道，无需设置
  /// </summary>
  /// <param name="config"></param>
  virtual void SetAppChannel(const TCDChannelConfig& config) = 0;

  /// <summary>
  /// 设置私有化模式，无私有化部署无需调用
  /// </summary>
  /// <param name="config">详见TCDPrivateConfig定义</param>
  virtual void SetPrivateConfig(const TCDPrivateConfig& config) = 0;

  /// <summary>
  /// 设置能力开启配置
  /// </summary>
  /// <param name="config">详见TCDAbilityConfig定义</param>
  virtual void SetAbilityConfig(const TCDAbilityConfig& config) = 0;

  /// <summary>
  /// 检测能力支持情况
  /// </summary>
  /// <param name="ability">详见TCDAbility定义</param>
  /// <returns>true表示支持，false 表示不支持</returns>
  virtual bool CheckAbility(TCDAbility ability) = 0;

  /// <summary>
  /// 关闭log上报
  /// </summary>
  virtual void DisableLogReport() = 0;

  /// <summary>
  /// 启动sdk
  /// </summary>
  /// <param name="config"></param>
  virtual void StartTCDEngine(const TCDSenderConfig& config) = 0;

  /// <summary>
  /// 停止SDK，调用后会清理投屏相关资源
  /// </summary>
  virtual void StopTCDEngine() = 0;

  /// <summary>
  /// 开始投屏
  /// </summary>
  /// <param name="config">详见TCDCastConfig定义</param>
  virtual void StartCast(const TCDCastConfig& config) = 0;

  /// <summary>
  /// 添加接收端
  /// </summary>
  /// <param name="config">详见TCDCastConfig定义</param>
  virtual void AddCast(const TCDCastConfig& config) = 0;

  /// <summary>
  /// 结束投屏
  /// </summary>
  virtual void StopCast() = 0;

  /// <summary>
  /// 暂停投屏
  /// </summary>
  /// <param name="pause"></param>
  virtual void PauseCast(bool pause) = 0;

  /// <summary>
  /// 获取sdk状态
  /// </summary>
  /// <returns>详见TCDEngineState定义</returns>
  virtual TCDEngineState GetEngineState() = 0;

  /// <summary>
  /// 获取投屏状态
  /// </summary>
  /// <returns>详见TCDEngineCastState定义</returns>
  virtual TCDEngineCastState GetCastState() = 0;

  /// <summary>
  /// 开关声音投放
  /// </summary>
  /// <param name="enable">true 开启 false 关闭</param>
  /// <returns>详见错误码定义</returns>
  virtual TCDError EnableAudio(bool enable) = 0;

  /// <summary>
  /// 开关扩展屏模式
  /// </summary>
  /// <param name="enable">true 开启 false 关闭</param>
  /// <returns>详见错误码定义</returns>
  virtual TCDError EnableExtendScreen(bool enable) = 0;

  /// <summary>
  /// 恢复投屏连接
  /// </summary>
  /// <param name="info">OnRecoveryNotify回调中附带的信息</param>
  virtual void RecoveryCast(const TCDRecoveryInfo& info) = 0;

	/// <summary>
	/// 更新鉴权信息
	/// </summary>
	/// <param name="corp_auth">鉴权信息</param>
  virtual void UpdateAuthInfo(const char* corp_auth) = 0;

  /// <summary>
  /// 网络检测
  /// </summary>
  virtual void StartCheckNetwork() = 0;

  /// <summary>
  /// 停止网络检测
  /// </summary>
  virtual void StopCheckNetwork() = 0;
};

/// <summary>
/// 获取sdk接口
/// </summary>
TCD_API TCDEngineSender* GetTCDSenderEngine();
}  // namespace tcd

#endif  // TCD_TCD_ENGINE_SENDER_H_
