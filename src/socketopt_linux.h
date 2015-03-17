#ifndef XD_SOCKETOPTIMP_LINUX_H
#define XD_SOCKETOPTIMP_LINUX_H

#include "types.h"
#include <endian.h>
#include <arpa/inet.h>

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

    // socket 操作
    // 设置无堵塞fd
    int32 setNonblocking(int32 fd);
    // 在fork时关闭fd
    int32 setCloseOnExec(int32 fd);
    // 创建无堵塞fd
    SOCKET createNonblockingOrDie();
    // 连接
    int32 connect(SOCKET fd, const sockaddr &addr);
    // 绑定
    int32 bind(SOCKET fd, const sockaddr &addr);
    // 监听
    int32 listen(SOCKET fd);
    // 接收
    SOCKET accept(SOCKET fd, sockaddr_in *addr);
    // 关闭
    void close(int32 fd);


}; // end namespace XDSpcketOpt

#endif // end xd_socketoptimp_linux_h
