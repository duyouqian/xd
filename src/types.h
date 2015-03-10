#ifndef XD_TYPES_H
#define XD_TYPES_H

// unsigned
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

// signed
typedef signed char         int8;
typedef signed short int    int16; typedef signed int          int32;
typedef signed long long    int64;

// max min num
#define MIN_uint8           ((uint8)    0x00)
#define MIN_uin16           ((uint16)   0x0000)
#define MIN_uint32          ((uint32)   0x00000000)
#define MIN_uint64          ((uint64)   0x0000000000000000)
#define MIN_int8            ((int8)     128)
#define MIN_int16           ((int16)    32768)
#define MIN_int32           ((int32)    0x80000000)
#define MIN_int64           ((int64)    0x8000000000000000)

#define MAX_uint8           ((uint8)    0xff)
#define MAX_uin16           ((uint16)   0xffff)
#define MAX_uint32          ((uint32)   0xffffffff)
#define MAX_uint64          ((uint64)   0xffffffffffffffff)
#define MAX_int8            ((int8)     0x7f)
#define MAX_int16           ((int16)    0x7fff)
#define MAX_int32           ((int32)    0x7fffffff)
#define MAX_int64           ((int64)    0x7fffffffffffffff)

#define MIN_float           (1.175494351e-38F)
#define MAX_float           (3.402823466e+38F)
#define MIN_double          (2.2250738585072014e-308)
#define MAX_double          (1.7976931348623158e+308)

#define MIN(a, b)           (a) < (b) ? (a) : (b)
#define MAX(a, b)           (a) > (b) ? (a) : (b)

// errorcode
typedef int32 XDErrorCode;


// 定义 NULL
const class nullptr_t_t
{
public:
    template<class T> operator T*() const { return 0; }
    template<class C, class T> operator T C::*() { return 0; }
private:
    void operator& () const;
} nullptr_t = {};

// pthread 执行对象
typedef void* (*ThreadEntryPoint)(void*);

#ifndef NULL
    #define NULL nullptr_t
#endif

#include <assert.h>
#define check(exp) assert(exp)

// 强制内联
#ifdef _MSC_VER_ // for MSVC
    #define FORCEINLINE __forceinline
#elif defined __GNUC__ // for gcc on Linux/Apple OS X
    #define FORCEINLINE __inline__ __attribute__((always_inline))
#else
    #define FORCEINLINE
#endif

#ifdef __unix__
    // unix or linux
    #if defined(_LINUX64) || defined(_LP64)
        #define PLATFORM_64BITS                 1
    #else
        #define PLATFORM_64BITS                 0 
    #endif
    #define STDCALL
    #define PTHREAD_NULL                        -1
#elif __APPLE__
    // mac os
    #define PLATFORM_64BITS                     0
    #define STDCALL
    #define PTHREAD_NULL -1
#else
    // win
    #define PLATFORM_64BITS                     0
    #define STDCALL                             __stdcall
    #define PTHREAD_NULL                        NULL
#endif


#endif // end xd_types_h
