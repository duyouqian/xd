#ifndef XD_TCP_CONNECTION_H
#define XD_TCP_CONNECTION_H

#include "noncopyable.h"
#include "callback.h"
#include "socket.h"
#include "inetaddr.h"
#include "types.h"

#include <string>
#include <memory>

class XDIOEventLoop;
class XDChannel;

// tcp 连接
class XDTcpConnection : public XDNoncopyable,
                        public std::enable_shared_from_this<XDTcpConnection>
{
public:
   // typedef XDSharedPtr<XDTcpConnection> XDTcpConnectionPtr;
    typedef std::shared_ptr<XDTcpConnection> XDTcpConnectionPtr;
public:
    explicit XDTcpConnection(XDIOEventLoop *loop,
                    std::string& name,
                    SOCKET fd,
                    const XDIpv4Addr &localAddr,
                    const XDIpv4Addr &peerAddr);
    ~XDTcpConnection();
    XDIOEventLoop* getLoop() const;
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
    //void send(XDBuffer *message);
    XDBuffer* getOutputBuffer();
    XDBuffer* getInputBuffer();
    
    void shutdown();
    void forceClose();
    //void forceCloseWithDelay(uint32 seconds);
    void setTcpNoDelay(bool on);
    
    // context
    void setContext(void *context);
    const void* getContext() const;
    void* getMutableContext();
    
    // ioevent callback
    void setConnectionCallBack(const XDIOEventConnectionCallBack &cb);
    void setMessageCallBack(const XDIOEventMessageCallBack &cb);
    void setWriteCompleteCallBack(const XDIOEventWriteCompleteCallBack &cb);
    void setHighWaterMarkCallBack(const XDIOEventHighWateMarkCallBack &cb, int32 highWaterMark);
    void setCloseCallBack(const XDIOEventCloseCallBack &cb);
    
    void connectEsEntablished();
    void connectDestroyed();

    //void setSelf(const XDTcpConnectionPtr &self);

private:
    enum XDState {state_disconnected, state_connecting, state_connected, state_disconnecting};
    //
    void sendInLoop(const void *message, int32 len);
    void shutdownInLoop();
    void forceCloseInLoop();
    void setState(XDState state);
    const char *stateToStr() const;
    // io 事件处理
    void handleRead(uint64 timestamp);
    void handleWrite();
    void handleError();
    void handleClose();
private:
    std::string name_;
    XDIOEventLoop *loop_;
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

    //XDTcpConnectionPtr self_;
};


#endif // end xd_tcp_connection_h
