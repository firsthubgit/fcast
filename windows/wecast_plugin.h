#ifndef PLUGINS_WECAST_PLUGIN_WINDOWS_H_
#define PLUGINS_WECAST_PLUGIN_WINDOWS_H_

#include <flutter_plugin_registrar.h>

#if defined(__cplusplus)
extern "C" {
#endif

void WecastPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

#if defined(__cplusplus)
}  // extern "C"
#endif

#endif  // PLUGINS_WECAST_PLUGIN_WINDOWS_H_
