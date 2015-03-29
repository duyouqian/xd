#ifndef XD_TCP_SERVER_H
#define XD_TCP_SERVER_H

#include "noncopyable.h"
#include "tcp_connection.h"
#include "inetaddr.h"
#include "reference_count.h"
#include "shared_pointer.h"
#include "types.h"
#include <functional>
#include <map>

class XDIOEventLoop;
class XDIOEventLoopThreadPool;
class XDAcceptor;

class XDTcpServer : public XDNoncopyable
{
    typedef std::function<void(XDIOEventLoop*)> XDIOEventLoopThreadInitCallBack;
public:
    XDTcpServer(XDIOEventLoop *loop,
                const XDIpv4Addr &addr,
                const char *name,
                bool isReuseport);
    ~XDTcpServer();
    void setNumThread(uint32 num);
    void start();
    XDSharedPtr<XDIOEventLoopThreadPool> getThreadPool();
    void setThreadInitCallBack(const XDIOEventLoopThreadInitCallBack &cb);
    void setConnectionCallBack(const XDIOEventConnectionCallBack &cb);
    void setMessageCallBack(const XDIOEventMessageCallBack &cb);
    void setWriteCompleteCallBack(const XDIOEventWriteCompleteCallBack &cb);

private:
    void newConnectionCallBack(FD fd, const XDIpv4Addr &addr);
    void removeConnectionCallBack(const XDTcpConnectionPtr &conn);
    void removeConnectionInLoop(const XDTcpConnectionPtr &conn);

private:
    // tcpserver name
    const std::string name_;
    // ip and port for string
    const std::string host_;
    // accept loop
    XDIOEventLoop *loop_;
    // accept
    XDSharedPtr<XDAcceptor> accept_;
    // ioeventloop thread
    XDSharedPtr<XDIOEventLoopThreadPool> threadPool_;
    // ioeventloopthread init callback
    XDIOEventLoopThreadInitCallBack threadInitCallBack_;
    // connection callback
    XDIOEventConnectionCallBack connectionCallBack_;
    // message callback
    XDIOEventMessageCallBack messageCallBack_;
    // write complete callback
    XDIOEventWriteCompleteCallBack writeCompleteCallBack_;
    // start atomic
    XDRefCounter started_;
    // 在loop线程
    int32 nextConnID_;
    // connections
    std::map<std::string, XDTcpConnectionPtr> connections_;
};

#endif // end xd_tcp_server_h
