/* 
** Copyright (c) 2014-2017 The XCast project. All rights reserved.
*/
#ifndef XCAST_ERROR_H_
#define XCAST_ERROR_H_

#include "xcast_base.h"

#if !defined(XCAST_ERROR)
# define XCAST_ERROR(label, value) XCAST_ERR_##label = value,
typedef enum xcast_error_e {
#else
typedef enum xc_error_e {
#endif
  XCAST_OK                  = 0,

  /* a generic failure occurred */
  XCAST_ERROR(FAILED, -1)

  XCAST_ERROR(AGAIN, -2)

  XCAST_ERROR(INVALID, -3)

  XCAST_ERROR(CANCELED, -4)

  /* unknown error */
  XCAST_ERROR(UNKNOWN, -5)

  /* config file is missed */
  XCAST_ERROR(CONFIG_NOT_FOUND, -6)

  /* signature key expired */
  XCAST_ERROR(KEY_EXPIRED, -8)

  /* signature key is invalid */
  XCAST_ERROR(KEY_INVALID, -12)

  /* protocol error occurred */
  XCAST_ERROR(PROTO_FAILED, -50)

  /* an asynchronous operation is not complete yet. this usually does not
  indicate a fatal error. typically this error will be generated as a
  notification to wait for some external notification that the operation
  finally completed */
  XCAST_ERROR(PENDING, -100)

  /* an operation is already in progress */
  XCAST_ERROR(BUSY, -101)

  /* an object is already exists */
  XCAST_ERROR(ALREADY_EXISTS, -103)

  /* an argument to the function is incorrect */
  XCAST_ERROR(INVALID_ARGUMENT, -104)

  /* the handle or file descriptor is invalid */
  XCAST_ERROR(INVALID_HANDLE, -105)

  /* an object cannot be found */
  XCAST_ERROR(NOT_FOUND, -106)

  /* an operation timed out */
  XCAST_ERROR(TIMED_OUT, -107)

  /* operation failed for xcast not started */
  XCAST_ERROR(NOT_STARTED, -110)

  /* operation failed because of unimplemented functionality */
  XCAST_ERROR(NOT_IMPLEMENTED, -111)

  /* there were not enough resources to complete the operation */
  XCAST_ERROR(INSUFFICIENT_RESOURCES, -112)

  /* memory allocation failed */
  XCAST_ERROR(OUT_OF_MEMORY, -113)

  /* operation not supported */
  XCAST_ERROR(NOT_SUPPORTED, -114)

  /* group audio/video banned */
  XCAST_ERROR(STREAM_BAN, -115)

  /* group video full */
  XCAST_ERROR(STREAM_VIDEO_FULL, -116)

  /* get interface server address failed */
  XCAST_ERROR(INTERFACE_SERVER_NOT_EXIST, -117)

  /* a previous operation is already in progress */
  XCAST_ERROR(OPERATION_IN_PROGRESS, -118)

  /* not a valid stream */
  XCAST_ERROR(INVALID_STREAM, -119)

  /* Init opensdk fail */
  XCAST_ERROR(INIT_FAIL, -120)

  /* device start fail */
  XCAST_ERROR(DEVICE_START_FAILED, -130)

  /* device stop fail */
  XCAST_ERROR(DEVICE_STOP_FAILED, -131)

  /* device volume set fail */
  XCAST_ERROR(DEVICE_VOLUME_FAILED, -132)

  /* Server kicked out as another client connected */
  XCAST_ERROR(SERVER_KICK_OUT, -150)

  /* The connection closed */
  XCAST_ERROR(CONNECTION_CLOSED, -151)

  /* Failed to connect */
  XCAST_ERROR(CONNECTION_FAILED, -152)

  /* App server failed to connect */
  XCAST_ERROR(APP_SERVER_FAILED, -153)
  
  XCAST_ERROR(PERMISSION_DENIED, -201)

  XCAST_ERROR(INVALID_CONFIG, -202)

  XCAST_ERROR(CLOSED, -203)

  XCAST_ERROR(MORE_DATA, -204)

  XCAST_ERROR(DROPPED, -205)

  XCAST_ERROR(EOF, -206)
#if !defined(XCAST_ERROR)
} xcast_error;
#else
} xc_error;
#endif

#endif /* XCAST_ERROR_H_ */
