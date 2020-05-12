#include "wecast_plugin.h"

#include <windows.h>

#include <VersionHelpers.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>
#include <memory>
#include <sstream>

// #if defined(WIN32) || defined(_WIN32)
// #error Not Support x86
// #endif


#include "adapter.h"

namespace {

class WecastPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar *registrar);

  // Creates a plugin that communicates on the given channel.
  WecastPlugin(
      std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel);

  virtual ~WecastPlugin();

 private:
  // Called when a method is called on |channel_|;
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  // The MethodChannel used for communication with the Flutter engine.
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;

  // tcd::TCDEngineSender* sender_;
};

// static
void WecastPlugin::RegisterWithRegistrar(flutter::PluginRegistrar *registrar) {
  foo();

  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "wecast",
          &flutter::StandardMethodCodec::GetInstance());
  auto *channel_pointer = channel.get();

  auto plugin = std::make_unique<WecastPlugin>(std::move(channel));

  channel_pointer->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WecastPlugin::WecastPlugin(
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel)
    : channel_(std::move(channel))/*, sender_(0)*/ {}

WecastPlugin::~WecastPlugin(){};

void WecastPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

  // if (sender_ == 0) {
  //   sender_ = tcd::GetTCDSenderEngine();
  // }

  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    // The result returned here will depend on the app manifest of the runner.
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    flutter::EncodableValue response(version_stream.str());
    result->Success(&response);
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void WecastPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  // The plugin registrar owns the plugin, registered callbacks, etc., so must
  // remain valid for the life of the application.
  static auto *plugin_registrar = new flutter::PluginRegistrar(registrar);

  WecastPlugin::RegisterWithRegistrar(plugin_registrar);
}
