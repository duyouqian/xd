#include "common.h"

class Thread1Run : public XDIRunnable
{
public:
    uint32 run()
    {
        XDIpv4Addr addr(7000);
        XDLOG_minfo("%s", addr.getIpAndPort().c_str());
        XDSocket sock;
        sock.open();
        sock.bindAddress(&addr);
        sock.listen();
        XDIpv4Addr client;
        XDLOG_minfo("client:%s", client.getIpAndPort().c_str());
        int32 count = 3;
        while (count) {
            SOCKET newfd = sock.accept(&client);
            XDLOG_minfo("new clinet fd:%d, addr:%s", newfd, client.getIpAndPort().c_str());
            --count;
        }
        sock.close();
    }
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    Thread1Run tr1;
    XDIThread *thread = XDThread::create(&tr1, "Thread1Run");
    thread->waitForComplateion();
    XDLOG_close();
    return 0;
}
