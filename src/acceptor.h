#ifndef XD_ACCEPTOR_H
#define XD_ACCEPTOR_H

#include "shareable.h"
#include "socket.h"
#include "channel.h"
#include "inetaddr.h"
#include "types.h"

#include <functional>

class XDIOEventLoop;

class XDAcceptor : public XDShareable
{
public:
    typedef std::function<void(FD, const XDIpv4Addr&)> XDIOEventNewConnectionCallBack;
public:
    XDAcceptor(XDIOEventLoop *loop, const XDIpv4Addr &listenAddr, bool reuseport);
    ~XDAcceptor();

    bool listenning() const;
    void listen();
    
    void setNewConnectionCallBack(const XDIOEventNewConnectionCallBack &cb);
    
private:
    void handRead(uint64 timestamp);

private:
    // IOEventLoop
    XDIOEventLoop *loop_;
    // Socket
    XDSocket acceptorSocket_;
    // acceptorChannel
    XDChannel acceptorChannel_;
    // listenning
    bool listenning_;
    // 空闲FD
    FD idleFD_;
    // 新连接回调
    XDIOEventNewConnectionCallBack newConnectionCallBack_;
};

#endif // end xd_acceptor_h
