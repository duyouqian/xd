#ifndef XD_SOCKETOPTIMP_LINUX_H
#define XD_SOCKETOPTIMP_LINUX_H

#include "types.h"
#include <endian.h>

namespace XDSocketOpt
{
    // 主讲字节序转网络字节序
    FORCEINLINE uint64 hostToNetwork64(uint64 value)
    {
        return htobe64(value);
    }

    FORCEINLINE uint32 hostToNetwork32(uint32 value)
    {
        return htobe32(value);
    }

    FORCEINLINE uint16 hostToNetwork16(uint16 value)
    {
        return htobe16(value);
    }

    // 网络字节序转主机字节序
    FORCEINLINE uint64 networkToHost64(uint64 value)
    {
        return be64toh(value);
    }

    FORCEINLINE uint32 networkToHost32(uint32 value)
    {
        return be32toh(value);
    }

    FORCEINLINE uint16 networkToHost16(uint16 value)
    {
        return be16toh(value);
    }
}; // end namespace XDSpcketOpt

#endif // end xd_socketoptimp_linux_h
