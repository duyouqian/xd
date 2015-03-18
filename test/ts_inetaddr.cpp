#include "common.h"

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIpv4Addr addr("192.168.1.56", 7000);
    XDLOG_minfo("%s", addr.getIpAndPort().c_str());
    XDLOG_close();
    return 0;
}
