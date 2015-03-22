#ifndef XD_ACCEPTOR_H
#define XD_ACCEPTOR_H

#include "shareable.h"
#include "socket.h"
#include "socket_util.h"
#include "inetaddr.h"
#include "callback.h"
#include "event_loop.h"
#include "channel.h"
#include "types.h"

class XDAcceptorHandleRead;

class XDAcceptor : public XDShareable
{
    friend class XDAcceptorHandleRead;
public:
    XDAcceptor(XDIOEventLoop *loop, const XDIpv4Addr &listenAddr, bool reuseport);
    ~XDAcceptor();

    bool listenning() const;
    void listen();
    
    void setNewConnectionCallBack(XDIOEventNewConnectionCallBackPtr &cb);

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
    XDIOEventNewConnectionCallBackPtr newConnectionCallBack_;
};

#endif // end xd_acceptor_h
