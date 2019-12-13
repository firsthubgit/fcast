/*
** Copyright (c) 2014 The XCast project. All rights reserved.
*/
#ifndef XCAST_MEDIA_H_
#define XCAST_MEDIA_H_

#include "./include/xcast_const.h"
#include "./include/xcast_variant.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/* clang-format off */

typedef enum xc_auth_type_e {
  xc_auth_none = 0,
  xc_auth_manual,
  xc_auth_auto,
} xc_auth_type;

typedef enum xc_encrypt_type_e {
  xc_encrypt_tea = 1,
  xc_encrypt_tls,
} xc_encrypt_type;

/* xcast channel event */
typedef enum xc_channel_event_e {
  xc_channel_added = 1,               /* new channel added */
  xc_channel_updated,                 /* channel state updated */
  xc_channel_removed,                 /* channel removed */
  xc_channel_custom_event,            /* channel custom event */
} xc_channel_event;

/* xcast channel state */
typedef enum xc_channel_state_e {
  xc_channel_connecting = 1,          /* channel connecting */
  xc_channel_connected,               /* channel connected */
  xc_channel_closed,                  /* channel closed */
} xc_channel_state;

/* xcast channel stream event */
typedef enum xc_stream_event_e {
  xc_stream_added = 1,                /* new stream added */
  xc_stream_updated,                  /* stream state updated */
  xc_stream_removed,                  /* stream removed */
  xc_stream_input_changed,            /* stream input changed */
  xc_stream_media,                    /* stream media data coming */
  xc_stream_custom_event,             /* stream custom event */
} xc_stream_event;

/* xcast channel stream running state */
typedef enum xc_stream_state_e {
  xc_stream_stopped = 1,              /* stream stopped */
  xc_stream_running,                  /* stream is running */
} xc_stream_state;

/* xcast stream type */
typedef enum xc_stream_type_e {
  xc_stream_audio = 1,                /* audio stream */
  xc_stream_video,                    /* video stream */
  xc_stream_data,                     /* data stream */
} xc_stream_type;

/* xcast stream direction */
typedef enum xc_stream_direction_e {
  xc_stream_out = 1,                  /* send media data to remote host */
  xc_stream_in,                       /* receive media data from remote host */
} xc_stream_direction;

/* xcast video format id enumeration */
typedef enum xc_video_format_e {
  xc_video_format_none    = -1,       /* unkown video format */
  xc_video_format_argb32  = 0,        /* packed ARGB, 32 bits ARGBARGB... */
  xc_video_format_bgra32,             /* packed BGRA, 32 bits BGRABGRA... */
  xc_video_format_abgr32,             /* packed ABGR, 32 bits ABGRABGR... */
  xc_video_format_rgba32,             /* packed RGBA, 32 bits RGBARGBA... */
  xc_video_format_rgb24,              /* packed RGB, 24 bits RGBRGB... */
  xc_video_format_bgr24,              /* packed BGR, 24 bits BGRBGR... */
  xc_video_format_i420,               /* planar YUV 4:2:0, 12 bits */
  xc_video_format_i422,               /* planar YUV 4:2:2, 16 bits */
  xc_video_format_i444,               /* planar YUV 4:4:4, 24 bits */
  xc_video_format_nv12,               /* interleaved chroma YUV 4:2:0, 12 bits, YYYYYYYY... UVUV... */
  xc_video_format_nv21,               /* interleaved chroma YUV 4:2:0, 12 bits, YYYYYYYY... VUVU... */
  xc_video_format_nv16,               /* interleaved chroma YUV 4:2:2, 16 bits, YYYYYYYY... UVUVUVUV... */
  xc_video_format_nv61,               /* interleaved chroma YUV 4:2:2, 16 bits, YYYYYYYY... VUVUVUVU... */
  xc_video_format_yv12,               /* planar YVU 4:2:0, 12 bits */
  xc_video_format_yv16,               /* planar YVU 4:2:2, 16 bits */
  xc_video_format_yv24,               /* planar YVU 4:4:4, 24 bits */
  xc_video_format_yuyv,               /* packed YUV 4:2:2, 16 bits, Y0 Cb Y1 Cr */
  xc_video_format_yvyu,               /* packed YUV 4:2:2, 16 bits, Y0 Cr Y1 Cb */
  xc_video_format_uyvy,               /* packed YUV 4:2:2, 16 bits, Cb Y0 Cr Y1 */
  xc_video_format_vyuy,               /* packed YUV 4:2:2, 16 bits, Cr Y0 Cb Y1 */
  xc_video_format_ayuv,               /* packed AYUV 4:4:4:4, 32 bits */
  xc_video_format_i420f,              /* planar YUV 4:2:0, 12 bits, full range (JPEG) */
  xc_video_format_i422f,              /* planar YUV 4:2:2, 16 bits, full range (JPEG) */
  xc_video_format_i444f,              /* planar YUV 4:4:4, 24 bits, full range (JPEG) */
  xc_video_format_nv12f,              /* interleaved chroma YUV 4:2:0, 12 bits, full range (JPEG) */
  xc_video_format_i010,               /* planar YUV 4:2:0, 10 bits per component, ITU-R BT.601 */
  xc_video_format_h010,               /* planar YUV 4:2:0, 10 bits per component, ITU-R BT.709 */
  xc_video_format_i400,               /* 8 bits gray */
  xc_video_format_max,                /* max video format */
  
  xc_video_format_j420   = xc_video_format_i420f,
  xc_video_format_j422   = xc_video_format_i422f,
  xc_video_format_j444   = xc_video_format_i444f,
  xc_video_format_gray8  = xc_video_format_i400,
  xc_video_format_y8     = xc_video_format_i400,
} xc_video_format;

/* xcast video format id enumeration */
typedef enum xc_audio_format_e {
  xc_audio_format_none  = -1,         /* unkown audio format */
  xc_audio_format_u8    = 0,          /* unsigned 8 bits */
  xc_audio_format_s16,                /* signed 16 bits */
  xc_audio_format_s32,                /* signed 32 bits */
  xc_audio_format_s64,                /* signed 64 bits */
  xc_audio_format_f32,                /* float */
  xc_audio_format_f64,                /* double */
  xc_audio_format_u8p,                /* unsigned 8 bits, planar */
  xc_audio_format_s16p,               /* signed 16 bits, planar */
  xc_audio_format_s32p,               /* signed 32 bits, planar */
  xc_audio_format_s64p,               /* signed 64 bits, planar */
  xc_audio_format_f32p,               /* float, planar */
  xc_audio_format_f64p,               /* double, planar */
  xc_audio_format_max,                /* max audio format */
} xc_audio_format;

/* xcast image format id enumeration */
typedef enum xc_image_format_e {
  xc_image_format_none    = -1,                       /* unkown image format */
  xc_image_format_argb32  = xc_video_format_argb32,   /* packed ARGB, 32 bits ARGBARGB... */
  xc_image_format_bgra32  = xc_video_format_bgra32,   /* packed BGRA, 32 bits BGRABGRA... */
  xc_image_format_rgb24   = xc_video_format_rgb24,    /* packed RGB, 24 bits RGBRGB... */
  xc_image_format_bgr24   = xc_video_format_bgr24,    /* packed BGR, 24 bits BGRBGR... */
  xc_image_format_max,                                /* max image format */
} xc_image_format;

/* xcast data format id enumeration */
typedef enum xc_data_format_e {
  xc_data_format_none     = -1,                        /* unkown data format */
  xc_data_format_variant  = 0,                         /* xcast_variant_t */
} xc_data_format;

/* xcast media data type */
typedef enum xc_media_data_type_e {
  xc_media_data_none      = -1,       /* unkown media data type */
  xc_media_data_memory    = 0,        /* memory, CPU accessible */
  xc_media_data_d3d9,                 /* offscreen Direct3DSurface9 */
  xc_media_data_d3d11,                /* D3D11Texture2D */
  xc_media_data_dib,                  /* DIB Section */
  xc_media_data_cuda,                 /* CUDA */
  xc_media_data_gl,                   /* OpenGL */
  xc_media_data_gles,                 /* OpenGL ES */
  xc_media_data_pixelbuffer,          /* CVPixelBuffer */
  xc_media_data_metal,                /* MTLTexture */
  xc_media_data_custom,               /* custom data */
  xc_media_data_surfacetexture,       /* SurfaceTexture */
} xc_media_data_type;

typedef uint32_t xc_media_format;

/* xcast media frame type */
typedef enum xc_media_frame_type_e {
  xc_media_frame_unknown = 0,         /* unknown media frame type */
  xc_media_frame_audio,               /* audio frame type */
  xc_media_frame_video,               /* video frame type */
  xc_media_frame_image,               /* image frame type */
  xc_media_frame_text,                /* text frame type */
  xc_media_frame_layer,               /* layer frame type */
  xc_media_frame_data,                /* data frame type */
} xc_media_frame_type;

/* xcast media source types */
typedef enum xc_media_source_e {
  xc_media_source_unknown = 0,        /* unknown source type */
  xc_media_source_camera,             /* camera */
  xc_media_source_screen_capture,     /* screen capture */
  xc_media_source_mic,                /* microphone */
  xc_media_source_media_player,       /* media player */
  xc_media_source_speaker,            /* speaker */
} xc_media_source;

/* xcast video frame rotation enumeration */
typedef enum xc_rotation_e {
  xc_rotation_none = 0,               /* no rotation */
  xc_rotation_0    = 0,               /* 0 degree */
  xc_rotation_90,                     /* 90 degree */
  xc_rotation_180,                    /* 180 degree */
  xc_rotation_270,                    /* 270 degree */
} xc_rotation;

/* xcast video frame origin of row */
typedef enum xc_origin_e {
  xc_origin_top     = 0,              /* top-down */
  xc_origin_bottom  = 1,              /* bottom-up */
} xc_origin;

typedef enum xc_blendmode_e {
  xc_blendmode_srcover = 0,
  xc_blendmode_dstover,
  xc_blendmode_src,
  xc_blendmode_srcin,
  xc_blendmode_dstin,
  xc_blendmode_srcout,
  xc_blendmode_dstout,
  xc_blendmode_srcatop,
  xc_blendmode_dstatop,
  xc_blendmode_xor,
  xc_blendmode_and,
  xc_blendmode_mul,
} xc_blendmode;

typedef enum xc_map_lock_flag_e {
  xc_map_read   = 1,
  xc_map_write  = 2,
} xc_map_lock_flag;

/* clang-format on */

typedef struct xc_media_frame_s xcast_media_frame_t;

typedef struct xc_media_frame_mem_s xcast_media_frame_mem_t;

/* join channel */
xcast_export int32_t xcast_join_channel(const char *id, xcast_variant_t *params);
#define xcast_start_channel xcast_join_channel

/* leave channel */
xcast_export int32_t xcast_leave_channel(const char *id);
#define xcast_stop_channel xcast_leave_channel

/* add reference count for media frame */
xcast_export xcast_media_frame_t *xcast_media_frame_ref(xcast_media_frame_t *frame);

/* release media frame */
xcast_export void xcast_media_frame_unref(xcast_media_frame_t *frame);

/* create memory audio frame with buffer */
xcast_export xcast_media_frame_t *xcast_audio_frame_attach(xc_audio_format format, uint8_t num_channels,
                                                           uint32_t num_samples, uint32_t sample_rate,
                                                           uint8_t *data, uint32_t data_size);

/* create memory video frame with buffer, 1 byte alignment */
xcast_export xcast_media_frame_t *xcast_video_frame_attach(xc_video_format format, uint32_t width, uint32_t height,
                                                           uint8_t *data, uint32_t data_size);

/* create data frame with 'xcast_variant_t' */
xcast_export xcast_media_frame_t *xcast_data_frame_attach(xcast_variant_t* data);

/* get xcast_variant_t from data frame */
xcast_export xcast_variant_t *xcast_data_frame_variant_get(xcast_media_frame_t *frame);

/* create memory audio frame */
xcast_export xcast_media_frame_t *xcast_audio_frame_alloc(xc_audio_format format, uint8_t num_channels,
                                                          uint32_t num_samples, uint32_t sample_rate);

/* create memory video frame, 1 byte alignment */
xcast_export xcast_media_frame_t *xcast_video_frame_alloc(xc_video_format format, uint32_t width, uint32_t height);
  
/* create data frame */
xcast_export xcast_media_frame_t *xcast_data_frame_alloc();

/* get 'parent' of media frame */
xcast_export const char *xcast_media_frame_parent_get(xcast_media_frame_t *frame);

/* get 'source' of media frame */
xcast_export const char *xcast_media_frame_source_get(xcast_media_frame_t *frame);

/* get source type of media frame */
xcast_export xc_media_source xcast_media_frame_source_type_get(xcast_media_frame_t *frame);

/* get 'timestamp' of media frame */
xcast_export uint64_t xcast_media_frame_timestamp_get(xcast_media_frame_t *frame);

/* get 'referencable' of media frame */
xcast_export bool xcast_media_frame_referencable_get(xcast_media_frame_t *frame);

/* get data type of media frame */
xcast_export xc_media_data_type xcast_media_frame_data_type_get(xcast_media_frame_t *frame);

/* get type of media frame */
xcast_export xc_media_frame_type xcast_media_frame_type_get(xcast_media_frame_t *frame);

/* map media frame data to memory */
xcast_export int32_t xcast_media_frame_map(xcast_media_frame_t *frame, xcast_media_frame_mem_t *mem,
                                           uint32_t flag);

/* unmap media frame data */
xcast_export int32_t xcast_media_frame_unmap(xcast_media_frame_t *frame, uint32_t flag);

/* alloc xcast_media_frame_mem_t */
xcast_export xcast_media_frame_mem_t *xcast_media_frame_mem_alloc();

/* free xcast_media_frame_mem_t */
xcast_export void xcast_media_frame_mem_free(xcast_media_frame_mem_t * mem);

/* get number of planes of the memory mapping */
xcast_export uint8_t xcast_media_frame_mem_plane_count_get(xcast_media_frame_mem_t *mem);

/* get the pointer to the data of the plane at index in the memory mapping */
xcast_export uint8_t *xcast_media_frame_mem_data_get(xcast_media_frame_mem_t *mem, uint8_t index);

/* get row bytes of the plane at index in the memory mapping */
xcast_export uint32_t xcast_media_frame_mem_stride_get(xcast_media_frame_mem_t *mem, uint8_t index);

/* get height of the plane at index in the memory mapping */
xcast_export uint32_t xcast_media_frame_mem_height_get(xcast_media_frame_mem_t *mem, uint8_t index);

/* get format of audio frame */
xcast_export xc_audio_format xcast_audio_frame_format_get(xcast_media_frame_t *frame);

/* get number of channels in audio frame */
xcast_export uint8_t xcast_audio_frame_num_channels_get(xcast_media_frame_t *frame);

/* get number of audio samples (per channel) in audio frame */
xcast_export uint32_t xcast_audio_frame_num_samples_get(xcast_media_frame_t *frame);

/* get sample rate of audio frame */
xcast_export uint32_t xcast_audio_frame_sample_rate_get(xcast_media_frame_t *frame);

/* get format of video frame */
xcast_export xc_video_format xcast_video_frame_format_get(xcast_media_frame_t *frame);

/* get width of video frame */
xcast_export uint32_t xcast_video_frame_width_get(xcast_media_frame_t *frame);

/* get height of video frame */
xcast_export uint32_t xcast_video_frame_height_get(xcast_media_frame_t *frame);

/* get rotation of video frame */
xcast_export xc_rotation xcast_video_frame_rotation_get(xcast_media_frame_t *frame);

/* get left value of the cropping rectangle of video frame */
xcast_export uint32_t xcast_video_frame_crop_left_get(xcast_media_frame_t *frame);

/* get top value of the cropping rectangle of video frame */
xcast_export uint32_t xcast_video_frame_crop_top_get(xcast_media_frame_t *frame);

/* get right value of the cropping rectangle of video frame */
xcast_export uint32_t xcast_video_frame_crop_right_get(xcast_media_frame_t *frame);

/* get bottom value of the cropping rectangle of video frame */
xcast_export uint32_t xcast_video_frame_crop_bottom_get(xcast_media_frame_t *frame);

/* set 'source' of media frame */
xcast_export int32_t xcast_media_frame_source_set(xcast_media_frame_t *frame, const char *source);

/* set source type of media frame */
xcast_export int32_t xcast_media_frame_source_type_set(xcast_media_frame_t *frame, xc_media_source source_type);

/* set 'timestamp' of media frame */
xcast_export int32_t xcast_media_frame_timestamp_set(xcast_media_frame_t *frame, uint64_t timestamp);

/* set 'referencable' of media frame */
xcast_export int32_t xcast_media_frame_referencable_set(xcast_media_frame_t *frame, bool referencable);

/* set userdata of media frame */
xcast_export int32_t xcast_media_frame_userdata_set(xcast_media_frame_t *frame, void *userdata, xcast_free_pt free,
                                                    xcast_dup_pt dup);

/* set userdata of media frame with 'xc_variant_t' format */
xcast_export int32_t xcast_media_frame_userdata_variant_set(xcast_media_frame_t *frame, xcast_variant_t *data);

/* set rotation of video frame */
xcast_export int32_t xcast_video_frame_rotation_set(xcast_media_frame_t *frame, xc_rotation rotation);

/* transfer format of audio/video frame */
xcast_export int32_t xcast_media_frame_transfer(xcast_media_frame_t *src, xcast_media_frame_t *dst);

#if defined(__APPLE__)

#include <TargetConditionals.h>
#include <CoreVideo/CoreVideo.h>

/* create video frame with CVPixelBufferRef */
xcast_export xcast_media_frame_t *xcast_video_frame_pixelbuffer_attach(CVPixelBufferRef pixelbuffer);

/* get CVPixelBuffer from xc_media_frame_t */
xcast_export CVPixelBufferRef xcast_video_frame_pixelbuffer_get(xcast_media_frame_t *frame);

#endif

#ifdef __cplusplus
}
#endif

#endif /* XCAST_MEDIA_H_ */
