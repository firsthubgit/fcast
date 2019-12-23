/*
** Copyright (c) 2014-2018 The XCast project. All rights reserved.
*/
#ifndef XCAST_MEDIA_EXTENSION_H_
#define XCAST_MEDIA_EXTENSION_H_

#include "xcast.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/*
* start/stop default 'audio-out' stream
*/
xcast_export int32_t xcast_start_audio_stream(const char *channel);
xcast_export int32_t xcast_stop_audio_stream(const char *channel);
  
/*
* start/stop default 'video-out' stream
*/
xcast_export int32_t xcast_start_video_stream(const char *channel);
xcast_export int32_t xcast_stop_video_stream(const char *channel);
  
/*
* start/stop default camera preview
*/
xcast_export int32_t xcast_start_video_preview();
xcast_export int32_t xcast_stop_video_preview();

  
/*
* get property 'enabled' of camera
*/
xcast_export bool xcast_get_camera_enabled();
  
/*
* set property 'enabled' of camera, enable/disable all cameras
*/
xcast_export void xcast_set_camera_enabled(bool enabled);
  
/*
* get property 'enabled' of microphone
*/
xcast_export bool xcast_get_mic_enabled();
  
/*
* set property 'enabled' of microphone, enable/disable all microphones
*/
xcast_export void xcast_set_mic_enabled(bool enabled);
  
/*
* get volume of microphone
*/
xcast_export uint32_t xcast_get_mic_volume();
  
/*
* set volume of microphone
*/
xcast_export void xcast_set_mic_volume(uint32_t volume);
  
/*
* get property 'enabled' of speaker
*/
xcast_export bool xcast_get_speaker_enabled();
  
/*
* set property 'enabled' of speaker, enable/disable all speakers
*/
xcast_export void xcast_set_speaker_enabled(bool enabled);
  
/*
* get volume of speaker
*/
xcast_export uint32_t xcast_get_speaker_volume();
  
/*
* set volume of speaker
*/
xcast_export void xcast_set_speaker_volume(uint32_t volume);
  
/*
* get property 'enabled' of accompany
*/
xcast_export bool xcast_get_accompany_enabled();
  
/*
* set property 'enabled' of accompany, enable/disable accompany
*/
xcast_export void xcast_set_accompany_enabled(bool enabled);
    
#ifdef __cplusplus
}
#endif

#endif /* XCAST_MEDIA_EXTENSION_H_ */
