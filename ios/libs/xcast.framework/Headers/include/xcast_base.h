/*
** Copyright (c) 2014 The XCast project. All rights reserved.
*/
#ifndef XCAST_BASIC_TYPES_H_
#define XCAST_BASIC_TYPES_H_

#include <limits.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(XCAST_ENABLE_EXPORT)

#if defined(WIN32)

#if defined(XCAST_IMPLEMENTATION)
#define xcast_export __declspec(dllexport)
#else
#define xcast_export __declspec(dllimport)
#endif /* defined(XCAST_IMPLEMENTATION) */

#else /* defined(WIN32) */
#if defined(XCAST_IMPLEMENTATION)
#if ((defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__))
#define xcast_export __attribute__((visibility("default")))
#else
#define xcast_export
#endif
#else
#define xcast_export
#endif /* defined(XCAST_IMPLEMENTATION) */
#endif

#else /* defined(XCAST_ENABLE_EXPORT) */
#define xcast_export
#endif

typedef void *(*xcast_ref_pt)(void *data);
typedef void (*xcast_unref_pt)(void *data);
typedef void (*xcast_free_pt)(void *data);
typedef int32_t (*xcast_func_pt)(void *user_data, void *data);
typedef int32_t (*xcast_log_pt)(int32_t level, const char *content, uint32_t size);
typedef void *(*xcast_dup_pt)(void *data);

typedef struct xc_variant_s xcast_variant_t;
    
#ifndef xc_null
#ifdef __cplusplus
#ifndef nullptr
#define xc_null (0)
#else /* nullptr */
#define xc_null nullptr
#endif /* nullptr */
#else /* !__cplusplus */
#define xc_null ((void*) 0)
#endif /* !__cplusplus */
#endif

#ifdef __cplusplus
}
#endif


#endif /* XCAST_BASIC_TYPES_H_ */
