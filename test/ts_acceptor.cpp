#include "common.h"

class NewConnectCallBack : public XDIOEventNewConnectionCallBack
{
public:
    bool exec(FD fd, const XDIpv4Addr &addr)
    {
        XDLOG_minfo("[NewConnection] 新连接 fd:%d addr:%s", fd, addr.getIpAndPort().c_str());
        return true;
    }
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoop loop;
    XDIpv4Addr addr(7000);
    XDAcceptor accept(&loop, addr, true);
    XDSharedPtr<NewConnectCallBack> cb(new NewConnectCallBack());
    accept.setNewConnectionCallBack(cb);
    accept.listen();
    loop.loop();
    XDLOG_close();
    return 0;
}
