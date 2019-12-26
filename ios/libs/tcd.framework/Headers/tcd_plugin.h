#ifndef WMP_TCD_PLUGIN_H_
#define WMP_TCD_PLUGIN_H_

#include "base/base_header.h"
#include "tcd/tcd_router.h"

#include "tcd/tcd_engine_def.h"

#ifdef _WIN32
#ifdef TCD_PLUGIN_EXPORT
#define TCD_PLUGIN_API __declspec(dllexport)
#else
#define TCD_PLUGIN_API
#endif
#else
#define TCD_PLUGIN_API
#endif

BEGIN_NAMESPACE_TCD_PLUGIN

class TCDPluginLogger {
public:
  virtual ~TCDPluginLogger()                        = default;
  virtual void PrintLog(const char*        src,
                        int32_t            line,
                        const char*        func,
                        int32_t            level,
                        const std::string& content) = 0;
};

class TCD_PLUGIN_API TCDPlugin {
public:
  virtual ~TCDPlugin()                                                 = default;
  virtual void     Initialize(std::shared_ptr<TCD_ROUTER::TCDRouter> router,
                              std::shared_ptr<TCDPluginLogger>       logger) = 0;
  virtual void     Uninitialize()                                      = 0;
  virtual uint64_t GetVersion()                                        = 0;
};

END_NAMESPACE_TCD_PLUGIN

#define plugin_log_level_begin 0x00
#define plugin_log_level_cp 0x01
#define plugin_log_level_cpe 0x02
#define plugin_log_level_error 0x03
#define plugin_log_level_fault 0x04
#define plugin_log_level_info 0x05
#define plugin_log_level_warning 0x06
#define plugin_log_level_debug 0x07
#define plugin_log_level_end 0x08

template <typename T>
struct plugin_is_ptr {
  static const bool is = false;
};

template <typename T>
struct plugin_is_ptr<T*> {
  static const bool is = true;
};

template <typename T>
struct plugin_is_char_ptr {
  static const bool is = false;
};

template <>
struct plugin_is_char_ptr<const char*> {
  static const bool is = true;
};

template <>
struct plugin_is_char_ptr<char*> {
  static const bool is = true;
};

template <typename T>
struct plugin_is_null_ptr {
  static bool is(const T& log) { return false; }
};

template <typename T>
struct plugin_is_null_ptr<T*> {
  static bool is(const T* log) { return log == nullptr; }
};

class __logwriter_plugin {
public:
  __logwriter_plugin(std::shared_ptr<TCD_PLUGIN::TCDPluginLogger> context,
                     int32_t                                      console,
                     int32_t                                      level,
                     const char*                                  src,
                     int32_t                                      line,
                     const char*                                  func)
      : context_(context), console_(console), level_(level), line_(line), src_(src), func_(func) {}

  ~__logwriter_plugin() {
    if (auto it = context_.lock()) {
      it->PrintLog(src_.c_str(), line_, func_.c_str(), level_, write_.str());
    }
  }

  template <typename T>
  inline __logwriter_plugin& operator<<(const T& log) {
    if (plugin_is_ptr<T>::is) {
      if (plugin_is_null_ptr<T>::is(log)) {
        write_ << "nullptr";
      } else {
        if (plugin_is_char_ptr<T>::is) {
          write_ << log;
        } else {
          write_ << "0x" << std::hex << log;
        }
      }
    } else {
      write_ << log;
    }
    return *this;
  }

  inline __logwriter_plugin& operator<<(std::ostream& (*log)(std::ostream&) ) {
    write_ << log;
    return *this;
  }

protected:
  std::weak_ptr<TCD_PLUGIN::TCDPluginLogger>   context_;
  std::stringstream                            write_;
  std::string                                  src_;
  std::string                                  func_;
  int32_t                                      level_;
  int32_t                                      line_;
  int32_t                                      console_;
};

#define plugin_log_xxx_print (0)
#define plugin_log_xxx(context, xxx) \
  __logwriter_plugin(context, plugin_log_xxx_print, plugin_log_level_##xxx, __FILE__, __LINE__, __FUNCTION__)
#define plugin_log_cp plugin_log_xxx(log_context, cp)
#define plugin_log_cpe plugin_log_xxx(log_context, cpe)
#define plugin_log_error plugin_log_xxx(log_context, error)
#define plugin_log_fault plugin_log_xxx(log_context, fault)
#define plugin_log_info plugin_log_xxx(log_context, info)
#define plugin_log_warning plugin_log_xxx(log_context, warning)
#define plugin_log_debug plugin_log_xxx(log_context, debug)

#define plugin_check_log(ret_code, func)                      \
  (ret_code ? plugin_log_info << #func << " ret:" << ret_code \
            : plugin_log_error << #func << "ret:" << ret_code)

#endif  // WMP_TCD_PLUGIN_H_