#ifndef XD_CALLBACK_H
#define XD_CALLBACK_H

#include "shareable.h"
#include "shared_pointer.h"
#include "tcp_connection.h"
#include "buffer.h"
#include "inetaddr.h"
#include "types.h"

#include <functional>

class XDIOEventLoop;

// IOEvent
typedef std::function<void()> XDIOEventCallBack;
// IOEvent read
typedef std::function<void(uint64)> XDIOEventReadCallBack;
// IOEvent newConn
typedef std::function<void(FD, const XDIpv4Addr)> XDIOEventNewConnectionCallBack;
// IOEventLoop thread init
typedef std::function<void(XDIOEventLoop*)> XDIOEventLoopThreadInitCallBack;
// connection state callback
typedef std::function<void(const XDTcpConnectionPtr &)> XDIOEventConnectionCallBack;
// message callback;
typedef std::function<void(const XDTcpConnectionPtr &, XDBuffer *, uint64)> XDIOEventMessageCallBack;
// write complete callback
typedef std::function<void(const XDTcpConnectionPtr &)> XDIOEventWriteCompleteCallBack;

// 关闭回调
typedef std::function<void(const XDTcpConnectionPtr &)> XDIOEventCloseCallBack;

// 高水位标回调
typedef std::function<void(const XDTcpConnectionPtr &)> XDIOEventHighWateMarkCallBack;
//class XDFunction : public XDShareable
//{
//public:
//    virtual bool exec() = 0;
//};

// 无参数回调
//class XDIOEventCallBack : public XDFunction
//{
//public:
//    virtual bool exec() = 0;
//};

// 新连接回调
//class XDIOEventNewConnectionCallBack : public XDShareable
//{
//public:
//    virtual bool exec(FD fd, const XDIpv4Addr &addr) = 0;
//};

// 读回调
//class XDIOEventReadCallBack : public XDShareable
//{
//public:
//    virtual bool exec(uint64 timestamp) = 0;
//};

// IOEventLoop初始化回调
//class XDIOEventLoopThreadInitCallBack : public XDShareable
//{
//public:
//    virtual bool exec(XDIOEventLoop *loop) = 0;
//};

// IOEvent 回调
//class XDIOEventCallBack : public XDShareable
//{
//public:
//    // 新连接回调
//    virtual bool newConnectionCallBack(FD fd, const XDIpv4Addr &addr) { return true; }
//    // 读回调
//    virtual bool readCallBack(uint64 timestamp) { return true; }
//    // 写回调
//    virtual bool writeCallBack() { return true; }
//    // 关闭回调
//    virtual bool closeCallBack() { return true; }
//    // 错误回调
//    virtual bool errorCallBack() { return true; }
//    // 连接回调
//    virtual bool connectionCallBack(const XDTcpConnectionPtr &conn) { return true; }
//    // 关闭回调
//    virtual bool closeCallBack(const XDTcpConnectionPtr &conn) { return true; }
//    // 写完成回调
//    virtual bool writeCompleteCallBack(const XDTcpConnectionPtr &conn) { return true; }
//    // 高水位标回调
//    virtual bool hightWaterMarkCallBack(const XDTcpConnectionPtr &conn) { return true; }
//    // 消息回调
//    virtual bool messageCallBack(const XDTcpConnectionPtr &conn, XDBuffer *buff, uint64 timestamp) { return true; }
//
//};

//typedef XDSharedPtr<XDFunction> XDFunctionPtr;
//typedef XDSharedPtr<XDIOEventCallBack> XDIOEventCallBackPtr;
//typedef XDSharedPtr<XDIOEventNewConnectionCallBack> XDIOEventNewConnectionCallBackPtr;
//typedef XDSharedPtr<XDIOEventReadCallBack> XDIOEventReadCallBackPtr;
//typedef XDSharedPtr<XDIOEventLoopThreadInitCallBack> XDIOEventLoopThreadInitCallBackPtr;


#endif // end xd_callback_h
