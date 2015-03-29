#include "common.h"
#include <functional>

typedef std::function<void()> CallBack;

class Test : public XDShareable
{
public:
    void readCallBack()
    {
        XDLOG_minfo("Test::readCallBack");
    }
};

class Exec
{
public:
    void run(const CallBack &cb)
    {
        cb();
    }
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDSharedPtr<Test> t(new Test());
    Exec e;
    e.run((std::bind(&Test::readCallBack, t.get())));
    return 0;
}
