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
    // 接收 并返回clinet SOCKET id
    SOCKET accept(SOCKET fd, sockaddr_in *addr);
    // 关闭
    void close(int32 fd);
    // 普通读
    int32 read(int32 fd, void *buffer, int32 len);
    // 普通写
    int32 write(int32 fd, const void *buffer, int32 len);
    // 集中读
    int32 readv(int32 fd, const struct iovec *iov, int32 iovCnt);
    // 获取socket 错误码
    int32 getSockError(SOCKET fd);
    // 根据SOCKET 获取本地地址
    struct sockaddr_in getLocalAddrWithSock(SOCKET fd);
    // 根据SOCKET 获取对端地址
    struct sockaddr_in getPeerAddrWithSock(SOCKET fd);
    // 判断SOCKET是自己的连接
    bool isSelfConnect(SOCKET fd);
    // 判断缓冲区是否还有数据可读
    bool peek(SOCKET fd);
    // 设置SOCKET选项
    void setSockTcpNoDelay(SOCKET fd, const void *optval, int32 optlen);
    void setSockReuseAddr(SOCKET fd, const void *optval, int32 optlen);
    void setSockReusePort(SOCKET fd, const void *optval, int32 optlen);
    void setSockKeepAlive(SOCKET fd, const void *optval, int32 optlen);
    // 关闭时写
    void shutdownWrite(SOCKET fd);

}; // end namespace XDSpcketOpt

#endif // end xd_socketoptimp_linux_h
