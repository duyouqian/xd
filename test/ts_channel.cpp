#include "common.h"

class ReadCallBack : public XDIOEventReadCallBack
{
public:
    bool exec(uint64 timestamp)
    {
        XDLOG_minfo("[ReadCallBack] time:%lu", timestamp);
        return true;
    }
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoop loop_;
    XDChannel channel(&loop_, -1);

    XDIOEventReadCallBackPtr readCb(new ReadCallBack());
    channel.setReadCallBack(readCb);

    // 设置事件
    channel.setEvent(XDIOEventType_READ, true);
    channel.setEvent(XDIOEventType_WRITE | XDIOEventType_ERROR, true);

    channel.hasEvent(XDTimer::getCurTimestampByMS());

    XDLOG_close();
    return 0;
}
