#ifndef XD_TYPES_H
#define XD_TYPES_H

// unsigned
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

// signed
typedef signed char         int8;
typedef signed short int    int16;
typedef signed int          int32;
typedef signed long long    int64;

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

#ifndef NULL
    #define NULL nullptr_t
#endif

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
        #define PLATFORM_64BITS             1
    #else
        #define PLATFORM_64BITS            0 
    #endif
#elif __APPLE__
    // mac os
    #define PLATFORM_64BITS             0
#else
    // win
    #define PLATFORM_64BITS             0
#endif


#endif // end xd_types_h
