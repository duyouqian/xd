#include "common.h"

class ReadCallBack : public XDIOEventCallBack
{
public:
    bool exec()
    {
    }
};

int main(int argvc, char **argv)
{
    XDLOG_open("log", 0);
    XDSharedPtr<ReadCallBack> read(new ReadCallBack());
    XDLOG_close();
    return 0;
}
