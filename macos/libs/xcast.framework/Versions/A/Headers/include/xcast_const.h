/*
** Copyright (c) 2014-2018 The XCast project. All rights reserved.
*/
#ifndef XCAST_CONST_H
#define XCAST_CONST_H

/* do not print log level higher than specified */
typedef enum xc_log_level_e {
  xc_log_err = 2,
  xc_log_warn,
  xc_log_info,
  xc_log_debug,
  xc_log_verbose,
} xc_log_level;

/* xcast system event */
typedef enum xc_system_event_e {
  xc_system_normal = 1,
  xc_system_warning,
  xc_system_error,
  xc_system_fatal,
} xc_system_event;

/* xcast device running state */
typedef enum xc_device_type_e {
  xc_device_unknown = 0,              /* unknown device type */
  xc_device_camera,                   /* camera device */
  xc_device_screen_capture,           /* screen capture */
  xc_device_video_render,             /* video render device */
  xc_device_mic,                      /* microphone device */
  xc_device_speaker,                  /* speaker device */
  xc_device_media_player,             /* media player */
  xc_device_external,                 /* external device type */
  xc_device_accompany,                /* audio acompany device (discard) */
  xc_device_max,
} xc_device_type;

/* xcast device event */
typedef enum xc_device_event_e {
  xc_device_added = 1,                /* new device added */
  xc_device_updated,                  /* device state updated */
  xc_device_removed,                  /* device removed */
  xc_device_preview,                  /* device preview data coming */
  xc_device_failure,                  /* device has error */
  xc_device_preprocess,               /* device preprocess data coming (discard) */
  xc_device_activated,                /* device activated */
  xc_device_custom_event,             /* device custom event */
} xc_device_event;

/* xcast device running state */
typedef enum xc_device_state_e {
  xc_device_stopped = 1,
  xc_device_running,
} xc_device_state;

/* xcast render scaling mode */
typedef enum xc_render_scaling_e {
  xc_render_scaling_auto_size = 0,     /* original frame size and auto resize view */
  xc_render_scaling_fit,               /* scale width and height to fit window size */
  xc_render_scaling_center_inside,     /* scale width or height to fit window size, and keep aspect ratio */
  xc_render_scaling_center_crop,       /* scale width or height to fit window size, and keep aspect ratio */
} xc_render_scaling;

/* xcast event list */

/*
* xcast events notify path defines.
* register callbacks through 'xcast_handle_event()' can receive
* notifies from xcast, the notified event value is a 'xcast_variant_t' of 'vdict' type.
* // xcast event discription, ver 000.000.1
* // attention: a 'xcast_variant_t(vdict)' type is used in xcast event callback as parameter.
* // the event has the following format(item with "*" is a required item):
* //   "event path":{
* //     // from where the event come from
* //     "*src":"vstring",
* //     // type of the event
* //     "*type":"vstring",
* //     // event state
* //     "state":"vstring",
* //     // error code
* //     "err":"vint32",
* //     // error message
* //     "err-msg":"vstring",
* //     // other custom event items
* //     ...
* //   }
* //
* // "[]" means the value is an array or option list, "{}" means value has a 'vdict' type.
* //
*/

/*
* xcast system event
* "event.system":{
*   // from where the event come from:
*   "*src":"system",
*   // type of the event: warning,error,fatal
*   "*type":"xc_system_event",
*   // error code
*   "err":"vint32",
*   // error message
*   "err-msg":"vstring"
* },
*/
#define XC_EVENT_SYSTEM                     "event.system"

/*
* xcast channel event
* "event.channel":{
*   // from where the event come from: name of the channel
*   "*src":"vstring",
*   // type of the event, one of 'xc_channel_event'
*   "*type":"xc_channel_event",
*   // event state, one of 'xc_channel_state'
*   "state":"xc_channel_state",
*   // error code
*   "err":"vint32",
*   // error message
*   "err-msg":"vstring"
* },
*/
#define XC_EVENT_CHANNEL                    "event.channel"

/*
* xcast stream event
* "event.stream":{
*   // from where the event come from: stream name
*   "*source":"vstring",
*   // which channel the stream belong: channel name
*   "*channel":"vstring",
*   // type of the event, one of 'xc_stream_event'
*   "*type":"xc_stream_event",
*   // stream class, one of 'xc_stream_type'
*   "*class":"xc_stream_type",
*   // direction of the stream, one of 'xc_stream_direction'
*   "*direction":"xc_stream_direction",
*   // which user the stream belong
*   "*uin":"vuint64",
*   // index of the stream, start from 0
*   "*index":"vuint32",
*   // stream state, one of 'xc_stream_state'
*   "state":"xc_stream_state",
*   // stream active state
*   "active":"vbool",
*   // stream quality value, NLFLAG value of int_common.h
*   "quality":"vint32",
*   // error code
*   "err":"vint32",
*   // error message
*   "err-msg":"vstring",
*   // stream media data format, one of 'xc_media_format'
*   "format":"xc_media_format",
*   // stream media raw data
*   "data":"vbytes",
*   // media data size
*   "size":"vuint32",
*   // media data width
*   "width":"vuint32",
*   // media data height
*   "height":"vuint32",
*   // rotation of the media data
*   "rotate":[0,90,180,270],
*   // media source of the stream, one of 'xc_media_source'
*   "source-type":"xc_media_source"
* },
*/
#define XC_EVENT_STREAM                     "event.stream"

/*
* xcast device event
* "event.device":{
*   // from where the event come from: device name
*   "*source":"vstring",
*   // type of the event, one of 'xc_device_event'
*   "*type":"xc_device_preprocess",
*   // type of the device, one of 'xc_device_type'
*   "*class":"xc_device_type",
*   // device data format
*   "format":"xc_media_format",
*   // device data
*   "data":"vbytes",
*   // size of device data
*   "size":"vuint32",
*   // width of device data
*   "width":"vuint32",
*   // height of device data
*   "height":"vuint32",
*   // rotate of device data
*   "rotate":[0,90,180,270],
*   // audio device only, volume value of [0,100]
*   "volume":"vuint32",
*   // audio device only, dynamic volume value of [0,100]
*   "dynamic-volume":"vuint32",
*   // media file player only, player path
*   "player-path":"vstring",
*   // media file player only, file path
*   "file-path":"vstring",
*   // media file player only, current position
*   "current-pos":"vint64",
*   // media file player only, max position
*   "max-pos":"vint64",
*   // accompany only, source of accompany
*   "accompany-source":["none","system","process"],
*   // screen capture only, left margin of capture rect
*   "screen-left":"vint32",
*   // screen capture only, top margin of capture rect
*   "screen-top":"vint32",
*   // screen capture only, right margin of capture rect
*   "screen-right":"vint32",
*   // screen capture only, bottom margin of capture rect
*   "screen-bottom":"vint32",
*   // screen capture only, capture fps
*   "screen-fps":"vuint32",
*   // windows screen capture only, capture window handler
*   "screen-hwnd":"vint32"
* },
*/
#define XC_EVENT_DEVICE                     "event.device"

/*
* xcast device event
* "event.device.prepocess":{
*   // from where the event come from: device name
*   "*src":"vstring",
*   // type of the event, one of 'xc_device_event'
*   "*type":"xc_device_preprocess",
*   // type of the device, one of 'xc_device_type'
*   "*class":"xc_device_type",
*   // device data format
*   "format":"xc_media_format",
*   // device data
*   "data":"vbytes",
*   // size of device data
*   "size":"vuint32",
*   // width of device data
*   "width":"vuint32",
*   // height of device data
*   "height":"vuint32",
*   // rotate of device data
*   "rotate":[0,90,180,270]
* },
*/
#define XC_EVENT_DEVICE_PREPROCESS          "event.device.prepocess"

/*
* // xcast tips event
* "event.tips":{
*   // from where the event come from: "audio","video"
*   "*src":["audio","video"],
*   "*tips":"vstring"
* },
*/
#define XC_EVENT_STATISTIC_TIPS             "event.tips"


#define XC_EVENT_CUSTOM                     "event.custom"

/*
* // xcast exception event
* "event.exception":{
*   // naming follows the rules: audio.xxx-xxx, video.xxx-xxx, network.xxx-xxx,  system.xxx-xxx and so on
*   // first xxx is sub type of main type, for example, video.enc-xxx, audio.device-xxx
*   "*type":"vstring"
* },
*/
#define XC_EVENT_EXCEPTION                  "event.exception"



/*
* // xcast properties description, ver 000.000.1
* // attention: only 'xcast_variant_t' type is supported as property value in xcast.
* // property has the following format:
* //   "property path":{
* //     "property get":{
* //       // return value: item with "*" is required, while "null" means not exist.
* //       "return":[null, {other value}]
* //     },
* //     "property set":{
* //       // param value: item with "*" is required, while "null" means not exist.
* //       "params":[null, {other value}],
* //     }
* // "[]" means the value is an array or option list, "{}" means value has a 'vdict' type.
* //
*/

/* xcast channel property */

/*
* // channel property: get running channel list in xcast
* "channel":{
*   "get":{
*     "return":[
*       // "null" means not exist
*       null,
*       // array of channel names
*       "vstring"
*     ]
*   }
* },
*/
#define XC_CHANNEL                          "channel"

/*
* // channel property: query channel state with the given name
* "channel.%s.state":{
*   "get":{
*     "return":[
*       // "null" means not exist
*       null,
*       // channel state, one of 'xc_channel_state'
*       "xc_channel_state"
*     ]
*   }
* },
*/
#define XC_CHANNEL_STATE                    "channel.%s.state"

/*
* // display property: enable/disable external-render of display
* "channel.%s.display.external-render":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_DISPLAY_EXTERNAL_RENDER          "channel.%s.display.external-render"

/*
* // channel property: query stream list in a given channel
* "channel.%s.stream":{
*   "get":{
*     "return":[
*       // "null" means not exist
*       null,
*       // array of stream names in the channel
*       "vstring"
*     ]
*   }
* },
*/
#define XC_CHANNEL_STREAM                   "channel.%s.stream"

/*
* // stream property: start/stop named stream in a given channel
* "channel.%s.%s.enabled":{
*   "set":{
*     "params":{
*       // while 'true' is for enable, and 'false' is for disable
*       "*enabled":"vbool",
*       // optional, specify video size for video stream only
*       "size":["small","big"]
*     }
*   }
* },
*/
#define XC_STREAM_ENABLED                   "channel.%s.%s.enabled"

/*
* // stream property: query named stream state
* "channel.%s.%s.state":{
*   "get":{
*     // return stream state, one of 'xc_stream_state'
*     "return":"xc_stream_state"
*   }
* },
*/
#define XC_STREAM_STATE                     "channel.%s.%s.state"

/*
* // stream property: query named stream info
* "channel.%s.%s.info":{
*   "get":{
*   // which user the stream belong
*   "*uin":"vuint64",
*   // stream class, one of 'xc_stream_type'
*   "*class":"xc_stream_type",
*   // direction of the stream, one of 'xc_stream_direction'
*   "*direction":"xc_stream_direction"
*   }
* },
*/

#define XC_STREAM_INFO                      "channel.%s.%s.info"

/*
* // stream property: set input for a given stream in channel
* "channel.%s.%s.input":{
*   "get":{
*     "return":[
*       // no input found
*       null,
*       // input name, such as a mic or a camera
*       "vstring"
*     ]
*   },
*   "set":{
*     // input name to be set
*     "params":"vstring"
*   }
* },
*/
#define XC_STREAM_INPUT                     "channel.%s.%s.input"

/* xcast camera property */

/*
* // camera property: get available camera list in system
* "device.camera":{
*   "get":{
*     // array of camera names
*     "return":"vstring"
*   }
* },
*/
#define XC_CAMERA                           "device.camera"

/*
* // camera property: enable/disable all cameras in system
* "device.camera.enabled":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_CAMERA_ENABLED                   "device.camera.enabled"

/*
* // camera property: get/set default camera for xcast, the default camera
* // is used when camera not specified for a stream.
* "device.camera.default":{
*   "get":{
*     "return":[
*       // no default camera
*       null,
*       // default camera name
*       "vstring"
*     ]
*   },
*   "set":{
*     // camera to be set
*     "params":"vstring"
*   }
* },
*/
#define XC_CAMERA_DEFAULT                   "device.camera.default"


/*
* // camera property: enable/disable camera data preprocess, the camera data
* // is received through device event for preprocess when preprocess is on.
* // the camera data can be processed in the event callback which is run
* // in capture thread. do not forget to disable this preprocess when do not
* // need preprocess any more.
* // CAUTION: DO NOT modify the frame data address, frame format and frame
* // size but ONLY the content in the event callback.
* "device.camera.preprocess":{
*   "get":{
*     "return":[
*       // no camera exist
*       null,
*       // camera preprocess state
*       "vbool"
*     ]
*   },
*   "set":{
*     // camera preprocess state
*     "params":"vbool"
*   }
* },
*/
#define XC_CAMERA_PREPROCESS                "device.camera.preprocess"

/*
* // camera property: start/stop camera preview, the preivew data
* // is received through device preview event when preview on
* "device.camera.%s.preview":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_CAMERA_PREVIEW                   "device.camera.%s.preview"

/*
* // camera: capture setting
* "device.camera.%s.setting":{
*   "get":{
*     "return":{
*       "height":"vint32",
*       "width":"vint32",
*       "fps":"vuint32",    // capture fps, value of [1~30]
*     }
*   },
*   "set":{
*     "params":{
*       "height":"vint32",
*       "width":"vint32",
*       "fps":"vuint32",    // capture fps, value of [1~30]
*     }
*   }
* },
*/
#define XC_CAMERA_SETTING           "device.camera.%s.setting"

/*
* // camera property: query camera state
* "device.camera.%s.state":{
*   "get":{
*     "return":[
*       // no camera exist
*       null,
*       // camera state, one of 'xc_device_state'
*       "xc_device_state"
*     ]
*   }
* },
*/
#define XC_CAMERA_STATE                     "device.camera.%s.state"

/*
* // camera property: enable/disable external-render of camera
* "device.camera.%s.external-render":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_CAMERA_EXTERNAL_RENDER           "device.camera.%s.external-render"

/* xcast mic property */

/*
* // mic property: get available mic list in system
* "device.mic":{
*   "get":{
*     "return":[
*       // "null" means not exist
*       null,
*       // array of mic names
*       "vstring"
*     ]
*   }
* },
*/
#define XC_MIC                              "device.mic"

/*
* // mic property: enable/disable all mics in xcast
* "device.mic.enabled":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_MIC_ENABLED                      "device.mic.enabled"

/*
* // mic property: get/set default mic for xcast, the default mic
* // is used when mic not specified for a stream.
* "device.mic.default":{
*   "get":{
*     "return":[
*       // no mic exist
*       null,
*       // default mic name
*       "vstring"
*     ]
*   },
*   "set":{
*     // mic name to set
*     "params":"vstring"
*   }
* },
*/
#define XC_MIC_DEFAULT                      "device.mic.default"

/*
* // mic property: get/set audio scene
* "device.mic.scene":{
*   "get":{
*     // scene value of [0,8],
*     0: VoiceChat, 
*     1: MediaPlayAndRecord, 2: MediaPlayback, 3: MediaPlayAndRecordHighQuality, 4:VoicePlayback
*     GME( 5:GameHighQuality, 6:GameLangRengSha, 7:GamePlayback, 8:GameMoba)
*     "return":"vint32"
*   },
*   "set":{
*     "params":"vint32"
*   }
* },
*/
#define XC_MIC_SCENE                        "device.mic.scene"

/*
* // mic property: query state of the given mic
* "device.mic.%s.state":{
*   "get":{
*     "return":[
*       // no mic exist
*       null,
*       // mic state, one of 'xc_device_state'
*       "xc_device_state"
*     ]
*   }
* },
*/
#define XC_MIC_STATE                        "device.mic.%s.state"

/*
* // mic property: start/stop mic preview, audio captured by this mic
* // is played by the working speaker when preview on
* "device.mic.%s.preview":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_MIC_PREVIEW                      "device.mic.%s.preview"

/*
* // mic property: get/set mic volume
* "device.mic.%s.volume":{
*   "get":{
*     // volume value of [0,100], default is 100
*     "return":"vuint32"
*   },
*   "set":{
*     "params":"vuint32"
*   }
* },
*/
#define XC_MIC_VOLUME                       "device.mic.%s.volume"

/*
* // mic property: get mic dynamic volume
* "device.mic.%s.dynamic-volume":{
*   "get":{
*     // volume value of [0~100]
*     "return":"vuint32"
*   }
* },
*/
#define XC_MIC_DYNAMIC_VOLUME               "device.mic.%s.dynamic-volume"

/*
* // batvoice receive, init or uninit
* "device.mic.batvoice_setting":{
*   "set":{
*      "state":["init","uninit"],
*      "digital":"vbool",
*   }
* },
*/
#define XC_MIC_BATVOICE_SETTING              "device.mic.batvoice-setting"

/*
* // batvoice enable
* "device.mic.batvoice-enable":{
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_MIC_BATVOICE_ENABLE               "device.mic.batvoice-enable"
/* xcast speaker property */

/*
* // speaker property: get available speakers in system
* "device.speaker":{
*   "get":{
*     "return": [
*       // no speaker exist
*       null,
*       // array of speaker names
*       "vstring"
*     ]
*   }
* },
*/
#define XC_SPEAKER                          "device.speaker"

/*
* // speaker property: enable/disable all speakers in system
* "device.speaker.enabled":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_SPEAKER_ENABLED                  "device.speaker.enabled"

/*
* // speaker property: get/set default speaker for xcast, the default speaker
* // is used as the audio output in xcast.
* "device.speaker.default":{
*   "get":{
*     "return":[
*       // no speaker exist
*       null,
*       // default speaker name
*       "vstring"
*     ]
*   },
*   "set":{
*     // speaker name to set
*     "params":"vstring"
*   }
* },
*/
#define XC_SPEAKER_DEFAULT                  "device.speaker.default"

/*
* // speaker property: enable/disable earphone mode in xcast
* "device.speaker.earphone-mode":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_SPEAKER_EARPHONE_MODE            "device.speaker.earphone-mode"

/*
* // speaker property: query state of the given speaker
* "device.speaker.%s.state":{
*   "get":{
*     "return":[
*       // no speaker exist
*       null,
*       // speaker state, one of 'xc_device_state'
*       "xc_device_state"
*     ]
*   }
* },
*/
#define XC_SPEAKER_STATE                    "device.speaker.%s.state"

/*
* // speaker property: start/stop speaker preview, will start speaker as the working
* // one in xcast when preview on, the speaker is stopped when preview is off.
* "device.speaker.%s.preview":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_SPEAKER_PREVIEW                  "device.speaker.%s.preview"

/*
* // speaker property: get/set speaker volume
* "device.speaker.%s.volume":{
*   "get":{
*     // volume value of [0~100], default is 100
*     "return":"vuint32"
*   },
*   "set":{
*     "params":"vuint32"
*   }
* },
*/
#define XC_SPEAKER_VOLUME                   "device.speaker.%s.volume"

/*
* // speaker property: get speaker dynamic volume
* "device.speaker.%s.dynamic-volume":{
*   "get":{
*     // volume value of [0~100]
*     "return":"vuint32"
*   }
* },
*/
#define XC_SPEAKER_DYNAMIC_VOLUME           "device.speaker.%s.dynamic-volume"

/*
* // speaker property: get/set speaker system volume
* "device.speaker.%s.system-volume":{
*   "get":{
*     // volume value of [0~100]
*     "return":"vuint32"
*   }
*   "set":{
*     // volume value of [0~100]
*     "params":"vuint32"
*   }
* },
*/
#define XC_SPEAKER_SYSTEM_VOLUME           "device.speaker.%s.system-volume"

/*
 * // speaker property: get/set speaker master volume
 * "device.speaker.%s.system-volume":{
 *   "get":{
 *     // volume value of [0~100]
 *     "return":"vuint32"
 *   }
 *   "set":{
 *     // volume value of [0~100]
 *     "params":"vuint32"
 *   }
 * },
 */
#define XC_SPEAKER_MASTER_VOLUME "device.speaker.master-volume"
/*
* // batvoice send, init or uninit
* "device.speaker.batvoice_setting":{
*   "set":{
*      "state":["init","uninit"],
*      "isDigital":"vbool"
*      "content":"vstring"
*   }
* },
*/
#define XC_SPEAKER_BATVOICE_SETTING              "device.speaker.batvoice_setting"

/*
* // batvoice enable
* "device.speaker.batvoice_enable":{
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_SPEAKER_BATVOICE_ENABLE               "device.speaker.batvoice_enable"

/* xcast external device property */

/*
* // external device property: get available external device list in xcast
* "device.external":{
*   "get":{
*     "return": [
*       // no external device exist
*       null,
*       // array of device names
*       "vstring"
*     ]
*   }
* },
*/
#define XC_DEVICE_EXTERNAL                  "device.external"

/*
* // external device property: input external data into xcast
* "device.external.%s.input":{
*   "set":{
*     "params":{
*       // media-frame
*       "*media-frame":"xcast_media_frame_t",
*     }
*   }
* },
*/
#define XC_DEVICE_EXTERNAL_INPUT            "device.external.%s.input"

/*
* external device property: set external device source type
* "device.external.%s.type":{
*   "set":{
*     "params":[
*       // source type, one of 'xc_media_source'
*       "xc_media_source"
*     ]
*   }
* },
*/
#define XC_DEVICE_EXTERNAL_TYPE            "device.external.%s.type"

/*
* // external device property: query external device state
* "device.external.%s.state":{
*   "get":{
*     "return":[
*       // no device exist
*       null,
*       // device state, one of 'xc_device_state'
*       "xc_device_state"
*     ]
*   }
* },
*/
#define XC_DEVICE_EXTERNAL_STATE            "device.external.%s.state"

/* xcast media player property */

/*
* // <TODO>media player property: enable/disable media player, use with
* // "device.media-player.setting" property.
* // format supported: *.aac,*.ac3,*.amr,*.ape,*.mp3,*.flac,*.midi,*.wav,*.wma,*.ogg,
* //   *.amv,*.mkv,*.mod,*.mts,*.ogm,*.f4v,*.flv,*.hlv,*.asf,*.avi,*.wm,*.wmp,
* //   *.wmv,*.ram,*.rm,*.rmvb,*.rpm,*.rt,*.smi,*.dat,*.m1v,*.m2p,*.m2t,*.m2ts,
* //   *.m2v,*.mp2v, *.tp,*.tpr,*.ts,*.m4b,*.m4p,*.m4v,*.mp4,*.mpeg4,*.3g2,*.3gp,
* //   *.3gp2,*.3gpp,*.mov,*.pva,*.dat,*.m1v,*.m2p,*.m2t,*.m2ts,*.m2v,*.mp2v,*.pss,
* //   *.pva,*.ifo,*.vob,*.divx,*.evo,*.ivm,*.mkv,*.mod,*.mts,*.ogm,*.scm,*.tod,*.vp6,
* //   *.webm,*.xlmv
* "device.media-player.enabled":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_MEDIA_PLAYER_ENABLED             "device.media-player.enabled"

/*
* // <TODO>media player property: media player setting
* "device.media-player.setting":{
*   "get":{
*     "return":{
*       "file":"vstring",                     // media file path
*       "state":["init","playing","paused"],  // play state
*       "pos":"vint64",                       // current play position
*       "max-pos":"vint64"                    // max position
*     }
*   },
*   "set":{
*     "params":{
*       "file":"vstring",                     // media file path
*       "restart":"vbool",                    // restart play
*       "state":["init","playing","paused"],  // play state
*       "pos":"vint64",                       // current play position
*       "max-pos":"vint64"                    // max position
*     }
*   }
* },
*/
#define XC_MEDIA_PLAYER_SETTING             "device.media-player.setting"

/*
* // <TODO>media player property: media file check
* "device.media-player.verify":{
*   "get":{
*     "params":{
*       "file":"vstring",                     // media file path
*       "loop-back":"vbool"                   // loop back mode
*     },
*     "return":{
*       "valid":"vbool",                      // 'true' when valid, otherwise 'false'
*       "audio-only":"vbool"                  // 'true' when the file only contain audio
*     }
*   }
* },
*/
#define XC_MEDIA_PLAYER_VERIFY              "device.media-player.verify"

/*
* // screen capture property: get available screen capture list in system
* "device.screen-capture":{
*   "get":{
*     // array of screen capture names
*     "return":"vstring"
*   }
* },
*/
#define XC_SCREEN_CAPTURE                   "device.screen-capture"

/*
* // screen capture: get/set default screen capture
* "device.screen-capture.default":{
*   "get":{
*     "return":[
*       // no screen capture exist
*       null,
*       // default screen capture name
*       "vstring"
*     ]
*   },
*   "set":{
*     // screen capture name to set
*     "params":"vstring"
*   }
* },
*/
#define XC_SCREEN_CAPTURE_DEFAULT           "device.screen-capture.default"

/*
* // screen capture: enable/disable screen preview
* "device.screen-capture.%s.preview":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/

#define XC_SCREEN_CAPTURE_PREVIEW           "device.screen-capture.%s.preview"

/*
* // screen capture: capture setting
* "device.screen-capture.%s.setting":{
*   "get":{
*     "return":{
*       "height":"vint32",
*       "width":"vint32",
*       "fps":"vuint32",    // capture fps, value of [1~30]
*     }
*   },
*   "set":{
*     "params":{
*       "height":"vint32",
*       "width":"vint32",
*       "fps":"vuint32",    // capture fps, value of [1~30]
*     }
*   }
* },
*/
#define XC_SCREEN_CAPTURE_SETTING           "device.screen-capture.%s.setting"

/*
* // screen-capture property: enable/disable external-render of screen-capture
* "device.screen-capture.%s.external-render":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_SCREEN_CAPTURE_EXTERNAL_RENDER   "device.screen-capture.%s.external-render"

/* xcast accompany property */

/*
* // <TODO>accompany property: enable/disable accompany
* "device.accompany.enabled":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_ACCOMPANY_ENABLED                "device.accompany.enabled"

/*
* // <TODO>accompany property: volume
* "device.accompany.volume":{
*   "get":{
*     // volume value of [0~100]
*     "return":"vuint32"
*   },
*   "set":{
*     // volume value of [0~100]
*     "params":"vuint32"
*   }
* },
*/
#define XC_ACCOMPANY_VOLUME                 "device.accompany.volume"

/*
* // <TODO>accompany property: dynamic volume
* "device.accompany.dynamic-volume":{
*   "get":{
*     // volume value of [0~100]
*     "return":"vuint32"
*   }
* },
*/
#define XC_ACCOMPANY_DYNAMIC_VOLUME         "device.accompany.dynamic-volume"

/*
* // <TODO>accompany property: get/set accompany setting
* "device.accompany.setting":{
*   "get":{
*     "return":{
*       // player path
*       "player":"vstring",
*       // audio file path
*       "file":"vstring",
*       // accompany source
*       "source":["none","system","process"]
*     }
*   },
*   "set":{
*     "params":{
*       // player path
*       "*player":"vstring",
*       // audio file path
*       "*file":"vstring",
*       // accompany source
*       "*source":["none","system","process"]
*     }
*   }
* },
*/
#define XC_ACCOMPANY_SETTING                "device.accompany.setting"

/* xcast preference */

/*
* // preference: auto create channel on server when channel not exist
* "preference.auto-channel":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "*params":"vbool"
*   }
* },
*/
#define XC_PREFERENCE_AUTO_CHANNEL          "preference.auto-channel"

/*
* // preference: get spear roles supported by xcast
* "preference.spear.roles":{
*   "get":{
*     // array of spear role name
*     "return":["vstring"]
*   }
* },
*/
#define XC_PREFERENCE_SPEAR_ROLES           "preference.spear.roles"

/*
* // preference: get current spear role name
* "preference.role":{
*   "get":{
*     // role name
*     "return":"vstring"
*   },
*   "set":{
*     // valid role name, one of "preference.spear.roles"
*     "*params":"vstring",
*     "return":null
*   }
* },
*/
#define XC_PREFERENCE_ROLE                  "preference.role"

/*
* // preference: get current spear role type
* "preference.role-type":{
*   "get":{
*     "params":null,
*     "return":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_ROLE_TYPE             "preference.role-type"

/*
* // preference: get/set external device count supported by xcast
* "preference.external-device":{
*   "get":{
*     // device count, default is 4.
*     "return":"vint32"
*   },
*   "set":{
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_EXTERNAL_DEVICE       "preference.external-device"


/*
* // preference: get/set spear config file path
* "preference.spear-config":{
*   "get":{
*     "return":"vstring"
*   },
*   "set":{
*     "*params":"vstring"
*   }
* },
*/
#define XC_PREFERENCE_SPEAR_CONFIG          "preference.spear-config"

/*
* // preference: get/set sso info
* "preference.net.sso":{
*   "get":{
*     "return":"vdict"
*   },
*   "set":{
*     "*params":"vdict"
*   }
* },
*/
#define XC_PREFERENCE_SSO          "preference.net.sso"

/*
* // preference: get/set camera setting, if the given resolution not supported by
* // camera, the nearest solution supported by camera will be used.
* "preference.camera":{
*   "get":{
*     "return":{
*       // camera frame data format
*       "*format":["xc_video_format_argb32", "xc_video_format_i420"],
*       // camera frame width
*       "*width":"vuint32",
*       // camera frame height
*       "*height":"vuint32",
*       // camera frame fps
*       "*fps":"vuint32",
*       // camera auto-rotate
*       "*auto-rotate":"vbool",
*       // camera auto-crop
*       "*auto-crop":"vbool"
*     }
*   },
*   "set":{
*     "*params":{
*       // camera frame data format
*       "format":["xc_video_format_argb32", "xc_video_format_i420"],
*       // camera frame width
*       "width":"vuint32",
*       // camera frame height
*       "height":"vuint32",
*       // camera frame fps
*       "fps":"vuint32",
*       // camera auto-rotate
*       "*auto-rotate":"vbool",
*       // camera auto-crop
*       "*auto-crop":"vbool"
*     }
*   }
* },
*/
#define XC_PREFERENCE_CAMERA                "preference.camera"

/*
 * // preference: get/set screen-capture setting
 * "preference.screen-capture":{
 *   "get":{
 *     "return":{
 *       // screen-capture frame data format
 *       "*format":["xc_video_format_argb32", "xc_video_format_i420"],
 *       // screen-capture frame width
 *       "*width":"vuint32",
 *       // screen-capture frame height
 *       "*height":"vuint32",
 *       // screen-capture frame fps
 *       "*fps":"vuint32",
 *     }
 *   },
 *   "set":{
 *     "*params":{
 *       // screen-capture frame data format
 *       "format":["xc_video_format_argb32", "xc_video_format_i420"],
 *       // screen-capture frame width
 *       "width":"vuint32",
 *       // screen-capture frame height
 *       "height":"vuint32",
 *       // screen-capture frame fps
 *       "fps":"vuint32",
 *     }
 *   }
 * },
 */
#define XC_PREFERENCE_SCREEN_CAPTURE        "preference.screen-capture"

/*
* // preference: get/set microphone setting
* "preference.mic":{
*   "get":{
*     "return":{
*     }
*   },
*   "set":{
*     "*params":{
*     }
*   }
* },
*/
#define XC_PREFERENCE_MIC                   "preference.mic"

/*
* // preference: get/set speaker setting
* "preference.speaker":{
*   "get":{
*     "return":{
*       "*channels":"vuint32",
*       "*rate":"vuint32",
*       "*bit":"vuint32",
*       "*span":"vuint32"
*     }
*   },
*   "set":{
*     "*params":{
*       "channels":"vuint32",
*       "rate":"vuint32",
*       "bit":"vuint32",
*       "span":vuint32
*     }
*   }
* },
*/
#define XC_PREFERENCE_SPEAKER               "preference.speaker"

/* xcast video preference */

/*
* // preference: get/set video encoder resolution
* "preference.video.encoder":{
*   "get":{
*     "return":{
*       // encode width
*       "*width":"vuint32",
*       // encode height
*       "*height":"vuint32",
*       // encode fps
*       "*fps":"vuint32"
*     }
*   },
*   "set":{
*     "*params":{
*       // encode width
*       "width":"vuint32",
*       // encode height
*       "height":"vuint32",
*       // camera frame fps
*       "fps":"vuint32"
*     }
*   }
* },
*/
#define XC_PREFERENCE_VIDEO_ENCODER     "preference.video.encoder"

/*
* // preference: get/set roi-enable settings
* "preference.video.roi-enable":{
*   "get":{
*     // value of [0~1], '0' means off
*     "return":"vint32"
*   },
*   "set":{
*     // value of [0~1], '0' means off
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_VIDEO_ROI "preference.video.roi-enable"

/*
* // preference: get/set stream settings
* "preference.stream.video":{
*   "get":{
*     "return":{
*       // encode width
*       "*width":"vuint32",
*       // encode height
*       "*height":"vuint32",
*       // encode fps
*       "*fps":"vuint32"
*     }
*   },
*   "set":{
*     "*params":{
*       // encode width
*       "width":"vuint32",
*       // encode height
*       "height":"vuint32",
*       // camera frame fps
*       "fps":"vuint32"
*     }
*   }
* },
*/
#define XC_PREFERENCE_STREAM_VIDEO "preference.stream.video"
#define XC_PREFERENCE_STREAM_SUBVIDEO "preference.stream.sub-video"

/*
* // <TODO>video preference: set video watermark param
* "preference.video.watermark":{
*   "set":{
*     "params":{
*       // watermark resolution
*       "*type":[
*         "320*240",    // resolution: 320*240
*         "480*360",    // resolution: 480*360
*         "640*480",    // resolution: 640*480
*         "640*368",    // resolution: 640*368
*         "960*540",    // resolution: 960*540
*         "1280*720",   // resolution: 1280*720
*         "192*144",    // resolution: 192*144
*         "320*180"     // resolution: 192*144
*       ],
*       // argb format watermark data
*       "*argb-data":"vbytes",
*       // watermark width
*       "*width":"vuint32",
*       // watermark height
*       "*height":"vuint32"
*     }
*   }
* },
*/
#define XC_PREFERENCE_VIDEO_WATERMARK       "preference.video.watermark"

/*
* // <TODO>video preference: skin smooth
* "preference.video.skin-smooth":{
*   "get":{
*     // value of [0~9], '0' means off
*     "return":"vint32"
*   },
*   "set":{
*     // value of [0~9], '0' means off
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_VIDEO_SKIN_SMOOTH     "preference.video.skin-smooth"

/*
* // video preference: enable/disable video auto rotation
* "preference.video.auto-rotate":{
*   "get":{
*     "return":"vbool"
*   },
*   "set":{
*     "params":"vbool"
*   }
* },
*/
#define XC_PREFERENCE_VIDEO_AUTO_ROTATE     "preference.video.auto-rotate"

/* xcast audio preference */

/*
* // audio preference: force client aec
* "preference.audio.force-aec":{
*   "get":{
*     // value of [-1(adaptive),0(off),1(on)]
*     "return":"vint32"
*   },
*   "set":{
*     // value of [-1(adaptive),0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_AEC             "preference.audio.force-aec"

/*
* // audio preference: force client agc
* "preference.audio.force-agc":{
*   "get":{
*     // value of [-1(adaptive),0(off),1(on)]
*     "return":"vint32"
*   },
*   "set":{
*     // value of [-1(adaptive),0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_AGC             "preference.audio.force-agc"

/*
* // audio preference: force client ans
* "preference.audio.force-ans":{
*   "get":{
*     // value of [-1(adaptive),0(off),1(on)]
*     "return":"vint32"
*   },
*   "set":{
*     // value of [-1(adaptive),0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_ANS             "preference.audio.force-ans"

/*
* // audio preference: open or close moba vad
* "preference.audio.moba_vad":{
*   "set":{
*     // value of [0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_MOBA_VAD        "preference.audio.moba_vad"

/*
* // audio preference: open or close bwe
* "preference.audio.bwe":{
*   "set":{
*     // value of [0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_BWE             "preference.audio.bwe"

/*
* // audio preference: open or close noisy-detect
* "preference.audio.noisy-detect":{
*   "set":{
*     // value of [0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_NOISY_DETECT    "preference.audio.noisy-detect"

/*
* // audio preference: open or close specific noise vad
* "preference.audio.spe_noise_vad":{
*   "set":{
*     // value of [0(off),1(on)]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_SPE_NOISE_VAD   "preference.audio.spe_noise_vad"

/*
* // audio preference: set grcng,0:OFF, others:level(dB)
* "preference.audio.grcng":{
*   "set":{
*     // value of [-60,0]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_GRCNG        "preference.audio.grcng"

/*
* // audio preference: set energy event gap, ms
* "preference.audio.energy-event-gap":{
*   "set":{
*     // value of [20,1000]
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_ENERGY_EVENT_GAP  "preference.audio.energy-event-gap"

/*
* // audio preference: abandon phone interrupt and recovery
* "preference.audio.abandon-phone-handle":{
*   "set":{
*     // value of [0,1], default 0
*     // 1 means abandon xcast phone interrupt and recovery
*     // 0 means open xcast phone interrupt and recovery
*     "*params":"vint32"
*   }
* },
*/
#define XC_PREFERENCE_AUDIO_ABANDON_PHONE_HANDLE  "preference.audio.abandon-phone-handle"

/*
 * // audio preference: abandon headphone auto mode change
 * "preference.audio.abandon-headphone-mode-change":{
 *   "set":{
 *     // value of [0,1], default 0
 *     // 1 means abandon xcast headphone auto mode change
 *     // 0 means open xcast headphone auto mode change
 *     "*params":"vint32"
 *   }
 * },
 */
#define XC_PREFERENCE_AUDIO_ABANDON_HEADPHONE_MODE_CHANGE  "preference.audio.abandon-headphone-mode-change"

/*
* // custom preference: max video bps
* "preference.custom.videomaxbps":{
*   "get":{
*     // value of [0~10000]
*     "return":"vint32"
*   },
*   "set":{
*     // value of [0~10000]
*     "*params":"vint32"
*   }
* }
*/
#define XC_PREFERENCE_CUSTOM_VIDEO_MAXBPS   "preference.custom.videomaxbps"


/*
* using with stop-audio, no param
*/
#define XC_AUDIO_START                      "driver.trae.start-audio"

/*
* using with start-audio, no param
*/
#define XC_AUDIO_STOP                       "driver.trae.stop-audio"

/*
* // audio preference: set jitter min delay
*   "set":{
*     // value of [80~400] best
*     "*params":"vint32"
*   }
*/
#define XC_AUDIO_SET_JITTER_MIN_DELAY      "driver.trae.jitter-mindelay"


#endif /* XCAST_CONST_H */
