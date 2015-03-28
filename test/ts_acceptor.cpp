#include "common.h"

    void newConnectionCallBack(FD fd, const XDIpv4Addr &addr)
    {
        XDLOG_minfo("[NewConnection] 新连接 fd:%d addr:%s", fd, addr.getIpAndPort().c_str());
    }

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoop loop;
    XDIpv4Addr addr(7000);
    XDAcceptor accept(&loop, addr, true);
    accept.setNewConnectionCallBack(std::bind(newConnectionCallBack, std::placeholders::_1, std::placeholders::_2));
    accept.listen();
    loop.loop();
    XDLOG_close();
    return 0;
}
