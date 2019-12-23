/*
** Copyright (c) 2018 The WeCast project. All rights reserved.
*/
#ifndef ROUTER_TCD_ROUTER_H_
#define ROUTER_TCD_ROUTER_H_

#include <functional>
#include <memory>
#include <string>

#include "base/base_header.h"
#include "base/chainable.h"
#include "base/log_writer.h"
#include "base/variant.h"

#define RESULT "result"
#define RESULT_MSG "result_msg"
#define SUCCEESS_DEFAULT_RESULT_MSG "requset success"
#define FAILED_DEFAULT_RESULT_MSG "requset failed"

#define ERR_NOT_FOUND 76001
#define ERR_TIMEOUT 76002

#define REG_ROUTER_GETREDUCE(uri, on_reduce)   \
  log_info << "reg router get reduce:" << uri; \
  TCD_ROUTER::GetRouter()->RegisterRouteGetReduce(typeid(this).name(), uri, on_reduce)

#define REG_ROUTER_GETREDUCEEX(uri, func)                                            \
  {                                                                                  \
    std::weak_ptr<this_class> weak_self(shared_from_this());                         \
    REG_ROUTER_GETREDUCE(                                                            \
        uri,                                                                         \
        [weak_self](const TCD_ROUTER::Request&                        param,         \
                    const TCD_ROUTER::TCDRouterOnGetResponseCallback& on_response) { \
          if (auto self = weak_self.lock()) {                                        \
            self->func(param, on_response);                                          \
          }                                                                          \
        });                                                                          \
  }

#define UNREG_ROUTER_GETREDUCE(uri)              \
  log_info << "unreg router get reduce:" << uri; \
  TCD_ROUTER::GetRouter()->UnRegisterRouteGetReduce(typeid(this).name(), uri)

#define REG_ROUTER_SYNC_GETREDUCE(uri, on_reduce)   \
  log_info << "reg router sync get reduce:" << uri; \
  TCD_ROUTER::GetRouter()->RegisterRouteSyncGetReduce(typeid(this).name(), uri, on_reduce)

#define REG_ROUTER_SYNC_GETREDUCEEX(uri, func)                                      \
  REG_ROUTER_SYNC_GETREDUCE(                                                        \
      uri, [this](const TCD_ROUTER::Request& param, TCD_ROUTER::Response& result) { \
        func(param, result);                                                        \
      });

#define UNREG_ROUTER_SYNC_GETREDUCE(uri)              \
  log_info << "unreg router sync get reduce:" << uri; \
  TCD_ROUTER::GetRouter()->UnRegisterRouteSyncGetReduce(typeid(this).name(), uri)

#define REG_ROUTER_BROADCAST(uri)             \
  log_info << "reg router broadcast:" << uri; \
  TCD_ROUTER::GetRouter()->RegisterRouteBroadcast(typeid(this).name(), uri)

#define UNREG_ROUTER_BROADCAST(uri)             \
  log_info << "unreg router broadcast:" << uri; \
  TCD_ROUTER::GetRouter()->UnRegisterRouteBroadcast(typeid(this).name(), uri)

#define REG_ROUTER_BROADCAST_RECEIVER(uri, on_broadcast) \
  log_info << "reg router broadcast receiver:" << uri;   \
  TCD_ROUTER::GetRouter()->RegisterRouteBroadcastReceiver(typeid(this).name(), uri, on_broadcast)

#define UNREG_ROUTER_BROADCAST_RECEIVER(uri)             \
  log_info << "unreg router broadcast receiver:" << uri; \
  TCD_ROUTER::GetRouter()->UnRegisterRouteBroadcastReceiver(typeid(this).name(), uri)

#define UNREG_ROUTER_ALL()               \
  log_info << "unreg all router"; \
  TCD_ROUTER::GetRouter()->UnRegisterAll(typeid(this).name())

#define ROUTER_GET(uri, param, on_response) \
  TCD_ROUTER::GetRouter()->Get(             \
      typeid(this).name(), __FILE__, __LINE__, __FUNCTION__, uri, param, on_response)

#define ROUTER_SYNC_GET(uri, param, result) \
  TCD_ROUTER::GetRouter()->SyncGet(         \
      typeid(this).name(), __FILE__, __LINE__, __FUNCTION__, uri, param, result)

#define ROUTER_SYNC_GET_OC(class_name, uri, param, result) \
TCD_ROUTER::GetRouter()->SyncGet(         \
class_name, __FILE__, __LINE__, __FUNCTION__, uri, param, result)

#define ROUTER_BROADCAST(uri, broadcast_data) \
  TCD_ROUTER::GetRouter()->BroadCast(         \
      typeid(this).name(), __FILE__, __LINE__, __FUNCTION__, uri, broadcast_data)

#define ROUTER_SET_URI_CONFIG(uri, config) \
  TCD_ROUTER::GetRouter()->SetUriConfig(typeid(this).name(), uri, config)

#define PLUGIN_REG_ROUTER_GETREDUCE(router, uri, on_reduce) \
  log_info << "reg router get reduce:" << uri;              \
  router->RegisterRouteGetReduce(typeid(this).name(), uri, on_reduce)


/*********for plugin class************/
#define PLUGIN_REG_ROUTER_GETREDUCEEX(router, uri, func)                             \
  {                                                                                  \
    std::weak_ptr<this_class> weak_self(shared_from_this());                         \
    REG_ROUTER_GETREDUCE(                                                            \
        uri,                                                                         \
        [weak_self](const TCD_ROUTER::Request&                        param,         \
                    const TCD_ROUTER::TCDRouterOnGetResponseCallback& on_response) { \
          if (auto self = weak_self.lock()) {                                        \
            self->func(param, on_response);                                          \
          }                                                                          \
        });                                                                          \
  }

#define PLUGIN_UNREG_ROUTER_GETREDUCE(router, uri) \
  log_info << "unreg router get reduce:" << uri;   \
  router->UnRegisterRouteGetReduce(typeid(this).name(), uri)

#define PLUGIN_REG_ROUTER_SYNC_GETREDUCE(router, uri, on_reduce) \
  log_info << "reg router sync get reduce:" << uri;              \
  router->RegisterRouteSyncGetReduce(typeid(this).name(), uri, on_reduce)

#define PLUGIN_REG_ROUTER_SYNC_GETREDUCEEX(router, uri, func)                       \
  REG_ROUTER_SYNC_GETREDUCE(                                                        \
      uri, [this](const TCD_ROUTER::Request& param, TCD_ROUTER::Response& result) { \
        func(param, result);                                                        \
      });

#define PLUGIN_UNREG_ROUTER_SYNC_GETREDUCE(router, uri) \
  log_info << "unreg router sync get reduce:" << uri;   \
  router->UnRegisterRouteSyncGetReduce(typeid(this).name(), uri)

#define PLUGIN_REG_ROUTER_BROADCAST(router, uri) \
  log_info << "reg router broadcast:" << uri;    \
  router->RegisterRouteBroadcast(typeid(this).name(), uri)

#define PLUGIN_UNREG_ROUTER_BROADCAST(router, uri) \
  log_info << "unreg router broadcast:" << uri;    \
  router->UnRegisterRouteBroadcast(typeid(this).name(), uri)

#define PLUGIN_REG_ROUTER_BROADCAST_RECEIVER(router, uri, on_broadcast) \
  log_info << "reg router broadcast receiver:" << uri;                  \
  router->RegisterRouteBroadcastReceiver(typeid(this).name(), uri, on_broadcast)

#define PLUGIN_UNREG_ROUTER_BROADCAST_RECEIVER(router, uri) \
  log_info << "unreg router broadcast receiver:" << uri;    \
  router->UnRegisterRouteBroadcastReceiver(typeid(this).name(), uri)

#define PLUGIN_UNREG_ROUTER_ALL(router) router->UnRegisterAll(typeid(this).name())

#define PLUGIN_ROUTER_GET(router, uri, param, on_response) \
  router->Get(typeid(this).name(), __FILE__, __LINE__, __FUNCTION__, uri, param, on_response)

#define PLUGIN_ROUTER_SYNC_GET(router, uri, param, result) \
  router->SyncGet(typeid(this).name(), __FILE__, __LINE__, __FUNCTION__, uri, param, result)

#define PLUGIN_ROUTER_SYNC_GET_OC(router, class_name, uri, param, result) \
router->SyncGet(class_name, __FILE__, __LINE__, __FUNCTION__, uri, param, result)

#define PLUGIN_ROUTER_BROADCAST(router, uri, broadcast_data) \
  router->BroadCast(typeid(this).name(), __FILE__, __LINE__, __FUNCTION__, uri, broadcast_data)

#define PLUGIN_ROUTER_SET_URI_CONFIG(router, uri, config) \
  router->SetUriConfig(typeid(this).name(), uri, config)

BEGIN_NAMESPACE_TCD_ROUTER

using Request = TCD_BASE::Variant;

using BroadCastData = TCD_BASE::Variant;

class Response : public TCD_BASE::Variant {
public:
  Response() : TCD_BASE::Variant(TCD_BASE::VariantType::kMap) {
    Get("result")     = int32_t(-1);
    Get("result_msg") = "";
  }

  Response(int32_t result, const std::string& result_msg)
      : TCD_BASE::Variant(TCD_BASE::VariantType::kMap) {
    SetResult(result, result_msg);
  }

  int32_t result() const { return Get("result").AsInt32(); }
  void    result(int32_t result) { Get("result") = result; }

  std::string result_msg() const { return Get("result_msg").AsStr(); }
  void        result_msg(const std::string& result_msg) { Get("result_msg") = result_msg; }

  void SetResult(int32_t result, const std::string& result_msg = "") {
    Get("result")     = result;
    Get("result_msg") = result_msg;
  }

  Response& Join(const Response& other) { 
    Variant::Join(other);
    return *this;
  }

  Variant* Raw() const { return (Variant*) (this); }
};

extern Response EMPTY_RESPONSE;

typedef std::function<void(Response& result)> TCDRouterOnGetResponseCallback;

typedef std::function<void(const Request& param, const TCDRouterOnGetResponseCallback& on_response)>
    TCDRouterOnGetProcessCallback;

typedef std::function<void(const Request& param, Response& result)>
    TCDRouterOnSyncGetProcessCallback;

typedef std::function<void(const BroadCastData& broadcast_data)> TCDRouterOnBroadcastCallback;

typedef std::string Handle;

struct UriConfig {
  int32_t log_level = 0x05;
  int32_t log_hit = 1;  // hit == 0 no log. hit == 1 log every times, hit = n > 1 log every n times.
  int64_t timeout = 10000;  // 10s timeout

  friend std::ostream& operator<<(std::ostream& out, const UriConfig& self) {
    out << "[log_level:" << self.log_level << " log_hit:" << self.log_hit
        << " timeout:" << self.timeout << "]";
    return out;
  }
};

class TCDRouter {
public:
  virtual ~TCDRouter() = default;

  virtual void RegisterRouteGetReduce(const Handle&                        handle,
                                      const std::string&                   uri,
                                      const TCDRouterOnGetProcessCallback& on_reduce) = 0;

  virtual void UnRegisterRouteGetReduce(const Handle& handle, const std::string& uri) = 0;

  virtual void RegisterRouteSyncGetReduce(const Handle&                            handle,
                                          const std::string&                       uri,
                                          const TCDRouterOnSyncGetProcessCallback& on_reduce) = 0;

  virtual void UnRegisterRouteSyncGetReduce(const Handle& handle, const std::string& uri) = 0;

  virtual void RegisterRouteBroadcast(const Handle& handle, const std::string& uri) = 0;

  virtual void UnRegisterRouteBroadcast(const Handle& handle, const std::string& uri) = 0;

  virtual void RegisterRouteBroadcastReceiver(const Handle&                       handle,
                                              const std::string&                  uri,
                                              const TCDRouterOnBroadcastCallback& on_broadcast) = 0;

  virtual void UnRegisterRouteBroadcastReceiver(const Handle&, const std::string& uri) = 0;

  virtual void UnRegisterAll(const Handle& handle) = 0;

  virtual void Get(const Handle&                         handle,
                   const char*                           src,
                   int32_t                               line,
                   const char*                           func,
                   const std::string&                    uri,
                   const Request&                        param,
                   const TCDRouterOnGetResponseCallback& on_response) = 0;

  virtual std::shared_ptr<TCD_BASE::ChainableTask> Get(
      const Handle&                      handle,
      const char*                        src,
      int32_t                            line,
      const char*                        func,
      const std::string&                 uri,
      const Request&                     param,
      const TCD_BASE::ChainableTaskCall& on_response) = 0;

  virtual void SyncGet(const Handle&      handle,
                       const char*        src,
                       int32_t            line,
                       const char*        func,
                       const std::string& uri,
                       const Request&     param,
                       Response&          result) = 0;

  virtual void BroadCast(const Handle&        handle,
                         const char*          src,
                         int32_t              line,
                         const char*          func,
                         const std::string&   uri,
                         const BroadCastData& broadcast_data) = 0;

  virtual void SetUriConfig(const Handle&      handle,
                            const std::string& uri,
                            const UriConfig&   config) = 0;
};

std::shared_ptr<TCDRouter> GetRouter();

inline Response MakeResult(int32_t result, const std::string& result_msg = "") {
  if (result_msg.empty()) {
    Response response;
    response.result(result);
    if (result) {
      response.result_msg(FAILED_DEFAULT_RESULT_MSG);
    } else {
      response.result_msg(SUCCEESS_DEFAULT_RESULT_MSG);
    }
    return response;
  } else {
    return Response(result, result_msg);
  }
}

inline void MakeResult(Response& response, int32_t result, const std::string& result_msg = "") {
  response.result(result);
  if (result_msg.empty()) {
    if (result) {
      response.result_msg(FAILED_DEFAULT_RESULT_MSG);
    } else {
      response.result_msg(SUCCEESS_DEFAULT_RESULT_MSG);
    }
  } else {
    response.result_msg(result_msg);
  }
}

END_NAMESPACE_TCD_ROUTER
#endif  // ROUTER_TCD_ROUTER_H_
