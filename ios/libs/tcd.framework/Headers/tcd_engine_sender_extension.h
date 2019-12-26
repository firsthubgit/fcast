/*
** Copyright (c) 2018 The WeCast project. All rights reserved.
*/

#ifndef WMP_TCD_ENGINE_SENDER_EXTENSION_H_
#define WMP_TCD_ENGINE_SENDER_EXTENSION_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "base/main_looper.h"
#include "base/timer.h"
#include "json/json.h"
#include "tcd/tcd_engine_extension_def.h"
#include "tcd/tcd_engine_sender.h"

namespace tcd {

struct TCDSenderConfigExt : public TCDSenderConfig {
  TCDSenderConfigExt(const TCDSenderConfig& config) {
    corp_id   = config.corp_id;
    nickname  = config.nickname;
    corp_auth = config.corp_auth;
    ins_id    = TCDInsid::kTCDInsidDefault;
  }

  friend std::ostream& operator<<(std::ostream& out, const TCDSenderConfigExt& self) {
    out << "[corp_id:" << self.corp_id << " nickname:" << self.nickname
        << " corp_auth:" << self.corp_auth
        << " ins_id:" << static_cast<int32_t>(self.ins_id) << "]";
    return out;
  }

  TCDInsid ins_id = TCDInsid::kTCDInsidDefault;
};

class TCDSenderListenerExtension : public TCDSenderListener {
public:
  virtual ~TCDSenderListenerExtension() = default;

  virtual void OnUserChangedExt(UserChangeTypeExt   change_type,
                                const TCDUserInner* change_list,
                                int32_t             change_size,
                                const TCDUserInner* total_list,
                                int32_t             total_size) {}

  virtual void OnC2CMessage(int32_t c2c_cmd, const std::string& message) = 0;

  virtual void OnStreamInfoUpdated(const std::string& discoveryInfo) {}
};

class TCDEngineSenderExtension : public TCDEngineSender {
public:
  virtual ~TCDEngineSenderExtension() = default;

  virtual void SetEnv(EnvConfig config) = 0;

  virtual void SetListener(TCDSenderListenerExtension* listener) = 0;

  virtual void StartTCDEngine(const TCDSenderConfigExt& ext_config) = 0;

  virtual void SendC2CMessage(
      TCDC2CMessageType  type,
      const std::string& data /*, const std::vector<std::string>& wmp_uid_list*/) = 0;

  virtual void AddEvent(uint32_t           key,
                        int32_t            code,
                        const std::string& extra_one = "",
                        const std::string& extra_two = "") = 0;

  virtual void FlushEvents() = 0;
};

TCD_API TCDEngineSenderExtension* GetTCDSenderEngineExt();
}  // namespace tcd

#endif  // WMP_TCD_ENGINE_SENDER_EXTENSION_H_
