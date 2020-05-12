#ifndef ADAPTER_H_
#define ADAPTER_H_

#ifdef ADAPTER_LIBRARY
#define XAPI __declspec(dllexport)
#else
#define XAPI __declspec(dllimport)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

XAPI int foo();

#if defined(__cplusplus)
}
#endif

#endif  // X64ADAPTER_H_
