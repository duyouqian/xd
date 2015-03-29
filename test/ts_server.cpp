#include "common.h"

void newConnectionCallBack(const XDTcpConnectionPtr &conn)
{
    XDLOG_mdebug("[Client] new connection");
    conn->send("hello", 5);
    XDTimer::safeSleepByS(2);
    //conn->shutdown();
}


int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoop loop;
    XDIpv4Addr addr(7000);
    XDTcpServer server(&loop, addr, "TestServer", true);
    server.setConnectionCallBack(std::bind(newConnectionCallBack, std::placeholders::_1));
    server.setNumThread(5);
    server.start();
    loop.loop();
    return 0;
}
