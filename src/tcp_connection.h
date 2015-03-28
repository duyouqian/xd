#ifndef XD_TCP_CONNECTION_H
#define XD_TCP_CONNECTION_H

#include "callback.h"
#include "shareable.h"
#include "shared_pointer.h"
#include "types.h"

// tcp 连接
class XDTcpConnection : public XDShareable
{
public:
    typedef XDSharedPtr<XDTcpConnection> XDTcpConnectionPtr;
public:
    XDTcpConnection(XDIOEvenLoop *loop,
                    std::string name,
                    SOCKET fd,
                    const XDIpv4Addr &localAddr,
                    const XDIpv4Addr &peerAddr);
    ~XDTcpConnection();
    XDIOEvent* getLoop() const;
    const std::string& name() const;
    const XDIpv4Addr& localAddress() const;
    const XDIpv4Addr& peerAddress() const;
    bool connected() const;
    bool disconnected() const;

    // tcpinfo
    //bool getTcpInfo(struct tcp_info*) const;
    //std::string getTcpInfoToStr() const;
    
    // send
    void send(const void *message, int32 len);
    void send(XDBuffer *message);
    void shutdown();

private:
    enum XDState {disconnected, connecting, connected, disconnecting};
    // io 事件处理
    void handleRead(uint64 timestamp);
    void handleWrite();
    void handleError();
    void handleClose();
private:
    std::string name_;
    XDIOEventLoop loop_;
    XDState state_;
    XDSharedPtr<XDSocket> socket_;
    XDSharedPtr<XDChannel> channel_;
    const XDIpv4Addr localAddr_;
    const XDIpv4Addr peerAddr_;
    // connection callback
    XDIOEventConnectionCallBack connectionCallBack_;
    // message callback
    XDIOEventMessageCallBack messageCallBack_;
    // write complete callback
    XDIOEventWriteCompleteCallBack writeCompleteCallBack_;
    // 高水位回调
    XDIOEventHighWateMarkCallBack highWateMarkCallBack_;
    // close callback
    XDIOEventCloseCallBack closeCallBack_;
    int32 highWaterMark_;
    // buffer
    XDBuffer inputBuffer_;
    XDBuffer outputBuffer_;
    // context
    void *context_;

    XDTcpConnectionPtr self_;
};

#endif // end xd_tcp_connection_h
