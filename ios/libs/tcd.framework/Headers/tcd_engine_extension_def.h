/*
** Copyright (c) 2018 The WeCast project. All rights reserved.
*/

#ifndef WMP_TCD_ENGINE_EXTENDSION_DEF_H_
#define WMP_TCD_ENGINE_EXTENDSION_DEF_H_

#include <iostream>
#include <string>
#include "tcd/tcd_engine_def.h"

namespace tcd {

enum class EnvConfig : int32_t { kRelease = 0, kPreRelease = 1, kDevelop = 2 };

enum class TCDInsid : int32_t {
  kTCDInsidDefault              = 0,
  kTCDInsidWindowsSender        = 1,
  kTCDInsidmacOSSender          = 2,
  kTCDInsidAndroidReceiver      = 3,
  kTCDInsidIOSSender            = 4,
  kTCDInsidWebSender            = 5,
  kTCDInsidAndroidSender        = 6,
  kTCDInsidWindowsReceiver      = 7,
  kTCDInsidWindowsSenderVisitor = 8,
  kTCDInsidmacOSSenderVisitor   = 9,

  kTCDInsidWindowsReceiverPaas = 20,
  kTCDInsidWindowsSenderPaas   = 21,
  kTCDInsidMacSenderPaas       = 22,
  kTCDInsidIOSSenderPaas       = 23,
  kTCDInsidAndroidSenderPaas   = 24,
  kTCDInsidAndroidReceiverPaas = 25,

  kTCDInsidWindowsSenderApp = 51,
  kTCDInsidMacOSSenderApp   = 52,
  kTCDInsidIOSSenderApp     = 53,
  kTCDInsidAndroidSenderApp = 54
};

enum class TCDChannelType : int32_t {
  kTCDChannelTypeUnknown = 0,
  kTCDChannelTypeCloud   = 1,
  kTCDChannelTypeWxwork  = 2,
};

enum class TCDUserState : int32_t {
  kTCDUserStateUnknown   = 0,
  kTCDUserStateAdd       = 1,
  kTCDUserStateDelete    = 2,
  kTCDUserStatePause     = 3,
  kTCDUserStatePlaying   = 4,
  kTCDUserStateHbTimeout = 5,
  kTCDUserStateMeet      = 6
};

enum class UserChangeTypeExt : int32_t {
  kUserChangeTypeExtEnter  = 1,
  kUserChangeTypeExtOut    = 2,
  kUserChangeTypeExtPause  = 3,
  kUserChangeTypeExtResume = 4,
  KUserChangeTypeExtUpdate = 5,
  KUserChangeTypeExtMeet   = 6
};

struct TCDUserInner {
  std::string corp_id;
  std::string nickname;
  std::string wmp_uid;
  TCDRole     role = kTCDRoleSender;
  std::string discovery_info;

  TCDInsid     ins_id = TCDInsid::kTCDInsidDefault;
  TCDUserState state  = TCDUserState::kTCDUserStateUnknown;
  std::string  platform;
  std::string  version;
  std::string  model;

  std::string app_id;
  std::string os_version;
  std::string lan_ip;
  std::string lan_port;
  std::string meeting_code;
  std::string meeting_password;
  std::string meeting_app_uid;
  std::string meeting_participate_id;
  std::string meeting_participate_key;

  friend std::ostream& operator<<(std::ostream& out, const TCDUserInner& self) {
    out << "wmpuid:" << self.wmp_uid << " state:" << static_cast<int32_t>(self.state) << " appid:" << self.app_id << " corpid:" << self.corp_id
        << " role:" << self.role << " insid:" << static_cast<int32_t>(self.ins_id) << " nickname:" << self.nickname << " platform:" << self.platform
        << " version:" << self.version << " model:" << self.model << " os_version:" << self.os_version << " lan_ip:" << self.lan_ip
        << " lan_port:" << self.lan_port << " discovery_info:" << self.discovery_info;
    return out;
  }

  bool operator==(const tcd::TCDUserInner& other) { return wmp_uid == other.wmp_uid; }

  bool operator<(const tcd::TCDUserInner& other) const { return wmp_uid < other.wmp_uid; }

  operator TCDUser() const {
    TCDUser user;
    user.corp_id        = corp_id.c_str();
    user.nickname       = nickname.c_str();
    user.tcd_uid        = wmp_uid.c_str();
    user.role           = role;
    user.discovery_info = discovery_info.c_str();
    return user;
  }

  operator bool() {
    if (wmp_uid.empty()) {
      return false;
    }

    return true;
  }
};

// 首页可运营信息（url+跑马灯展示内容）
struct OperationInfo {
  std::string url;    // picture url
  std::string text;   // description
  std::string extra;  // extra info (json string)

  friend std::ostream& operator<<(std::ostream& out, const OperationInfo& self) {
    out << "url:" << self.url << " text:" << self.text << " extra:" << self.extra;
    return out;
  }

  bool operator==(const OperationInfo& other) {
    if (this->url == other.url && this->text == other.text && this->extra == other.extra) {
      return true;
    }

    return false;
  }
};

struct PingTaskInner {
  std::string url;
  std::string description;
  int32_t     success_times;
  int32_t     total_ping_times;

  PingTaskInner(std::string i_url, std::string i_description, int32_t i_success_time, int32_t i_total_ping_time) {
    url              = i_url;
    description      = i_description;
    success_times    = i_success_time;
    total_ping_times = i_total_ping_time;
  }
};

enum class TCDExitReason : int32_t {
  kTCDExitNormal               = 0,  //  正常退出
  kTCDExitUnknow               = 1,
  kTCDExitTimeout              = 80009,  //  心跳超时
  kTCDExitByInviteFailed       = 80010,  //  邀请失败或超时退出
  kTCDTvExit                   = 80017,  //  tv主动断开
  kTCDTVNotEnter               = 80018,  //  tv邀请超时（5s）
  kTCDBookCover                = 80021,  //  合盖子退出
  kTCDTerminateApp             = 80022,  //  app被强杀
  kTCDXcastStreamTimeout       = 80023,  //  10s
  kTCDiOSScreenLock            = 80024,  //  ios锁屏
  kTCDReplayKitStop            = 80025,  //  iosReplayKit停止
  kTCDWinLock                  = 80031,  //  windows锁屏
  kTCDNoFirstFrameExit         = 80033,  //  无首帧倒计时退出
  kTCDUserChangeAfterReconnect = 80052,  //  重连后发现对端断开
  kTCDExitByGrabbed            = 80054,  //  抢投退出
};

// TODO(add cmd): c2c通道添加指令类型
enum class TCDC2CMessageType : int32_t {
  kC2CNone       = 0,  // sender <-> receiver 未知
  kS2RGuide      = 1,  // sender -> receiver 指引
  kR2SPauseXCast = 2,  // receiver -> sender 暂停XCast
  kR2SDLNAUpdate = 3,  // receiver -> mobile sender 更新DLNA信息
  kS2RQueryInfo  = 4   // mobile sender -> receiver 查询更新的信息
};

enum class TCDScreenReportTiming : int32_t {
  kTCDInvited             = 0,
  kTCDEnableExtendScreen  = 1,
  kTCDDisableExtendScreen = 2,
  kTCDExited              = 3,
};

////////////////////////////////////////////////////////////////////////////////////////
////  上报事件key
enum TCDEventKey {
  kApplicationStart          = 100,  // finished
  kHttpAccountLogin          = 101,  // finished
  kUpdateSelfAbility         = 102,  // finished(change websocket connect event -> update ability)
  kCastLogin                 = 103,  // finished
  kWebSocketConferenceCreate = 104,  // finished
  kHttpGetConferenceSign     = 105,  // finished
  kClientEnterConference     = 106,  // finished
  kXcastEnterConference      = 107,  // finished
  kInviteConference          = 108,  // finished
  kExitConference            = 109,  // finished
  kPauseConference           = 110,  // finished
  kEnableExtendScreen        = 111,  // (Windows), to be finished(apple)
  kStartConference           = 112,  // finished
  kDisableExtendScreen       = 113,  // (Windows), to be finished(apple)
  kSupportExtendScreen       = 114,  // (Windows), to be finished(apple)
  kiOSDataTransfer           = 115,  //
  kCountDown                 = 116,  //
  kCountExit                 = 117,  //
  kDriverInitial             = 125,  // finished
  kAddCast                   = 126,  // finished
  kExitWebSocket             = 127,  // useless
  kExitXCast                 = 128,  // useless
  kExtendByPPT               = 130,  //
  kScreenSizeReport          = 131,  // (Windows), to be finished(apple)
  kWlanConnect               = 134,  // (Windows)
  kWlanDisconnect            = 135,  // (Windows)
  kWlanRoaming               = 136,  // (Windows)
  kMacUserClickDocker        = 137,  //
  kConfigFileLost            = 138,  // useless
  kMaxhubExitByCmd           = 139,  //
  kMaxhubHideWindowByCmd     = 140,  //
  kMaxhubShowWindowByCmd     = 141,  //
  kZeroFps                   = 143,  // (Windows)
  kLimitMaxFps               = 146,  // (Windows)
  kLimitMaxFpsToFive         = 147,  // (Windows)
  kDisableDwm                = 148,  // (Windows)
  kResumeDwm                 = 149,  // (Windows)
  kMacGetXmlFailedInUpdate   = 151,  //
  kMacNoHigherVersion        = 152,  //
  kMacExistHigherVersion     = 153,  //
  kMacStartDownloadUpdate    = 154,  //
  kMacUpdateDownloadComplete = 155,  //
  kMacStartReleaseUpdate     = 156,  //
  kMacUpdateReleaseComplete  = 157,  //
  kMacUpdateInstallComplete  = 158,  //
  kMacTAPSHost               = 159,  //
  kMacOpenAirplay            = 160,  //
  kMacCloseAirplay           = 161,  //
  kMacAirplayLogoExist       = 180,  //
  kUpdateNow                 = 183,  // (Windows
  kUpdateLater               = 184,  // (Windows
  kForbiddenVersion          = 185,  // finished
  kXcastReportFinished       = 190,  //
  kBookCover                 = 191,  // (Windows)
  kGoingToSleep              = 192,  // (Windows)
  kStartWithoutLocalConfig   = 193,  //

  kNetworkUnstableTipsShow           = 194,  //
  kNetworkUnstableRecovered          = 195,  //
  kDisconnectToMainView              = 196,  // to be finished
  kRecoverFailed                     = 197,  //
  kShowReconnectWnd                  = 198,  //
  kReconnectSuccess                  = 199,  //
  kExitRoomInNetworkUnstableTips     = 202,  //
  kReceiverExitRoomByNetworkUnstable = 204,  //

  kConnectConfigCenterFailed = 205,  // finished
  kAppResUsage               = 206,  // finished 退出时cpu&ram使用情况
  kSwitchAudio               = 210,  // finished
  kPingFailed                = 211,  // finished
  kCloseAudioAfterAutoPlayed = 212,  //
  kSocketInitializFailed     = 215,  // finished
                                     // 162~164 used for 声波上报
                                     // 165 airplay switched report in ios_sender
  kWebsocketReonnected = 166,
  // 167~175 airplay/dlna report & player report in android receiver

  // 300+ meet
  kMeetSenderBegin   = 300,  // 发送端开始切会议
  kMeetSenderEnd     = 301,  // 发送端结束切会议
  kMeetReceiverBegin = 302,  // 接收端开始切会议
  kMeetReceiverEnd   = 303,  // 接收端结束切会议
  kMeetSupport       = 304,  // 是否支持支持切会议

  kCastClick    = 500,  //
  kPauseClick   = 501,  //
  kExtendClick  = 502,  //
  kMenuClick    = 503,  //
  kAddCastClick = 504,  //
  kExitClick    = 505,  //

  kiOSScreenLock                 = 506,  // 熄屏-屏幕熄灭
  kiOSScreenLockTipClick         = 507,  // 熄屏-点击熄屏提示条
  kiOSScreenLockAlert            = 508,  // 熄屏-点击熄屏提示条弹出对话框
  kiOSScreenLockGoSetting        = 509,  // 熄屏-熄屏对话框点击前往设置
  kiOSScreenLockAlertFirstLaunch = 510,  // 熄屏-首次强制弹出对话框
                                         // 506~510 used for 息屏上报
                                         // 511 toolbar scale change
                                         // 512~516 app icon click report in shangelila
  kLoadingCloseButtonClick = 517,        //
  kLoadingRetryButtonClick = 518,        //

  kNetworkUnstableWindowClicked  = 521,  // sdk & windows ok
  kReconnectConfirmButtonClicked = 522,  //
  kReconnectCancelButtonClicked  = 523,  //

  kAudioSwitchButtonClicked = 525,  //

  // 519~520 app forbid version click report

  kDWMBlocked        = 1000,  //
  kExtendScreenByPPT = 1001,  // useless
  kNetworkUnstable4  = 1002,  //
  kNetworkUnstable5  = 1003,  //
  kNetworkUnstable6  = 1004,  //
  kNetworkUnstableUI = 1005,  //

  kPingAbnormal         = 1200,  // finished
  kZeroFpsAbnormal      = 1201,  // finished
  kHeartBeatAbnormal    = 1202,  // finished
  kWlanStrengthAbnormal = 1203,  // finished
  kPowerStatusAbnormal  = 1204,  // finished
  kCpuUsageAbnromal     = 1205,  // finished
  kRamUsageAbnromal     = 1206,  // finished
  kLossRateAbnormal     = 1207,  // finished
  kRedoLossRateAbnormal = 1208,  // finished
};
const int32_t kAutoPingTimes = 1;  //  ping parameters
const int32_t kPingTimes     = 3;
const int32_t kPingTimeOut   = 2000;

}  // namespace tcd

#endif  // WMP_TCD_ENGINE_EXTENDSION_DEF_H_
