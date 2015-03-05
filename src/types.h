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

#endif // end xd_types_h
