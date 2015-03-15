#include "common.h"

class Test : public XDShareable
{
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDSharedPtr<Test> t;
    {
        XDSharedPtr<Test> p(new Test());
        XDLOG_minfo("p:%d\n", p->getCount());
        t = p;
        XDLOG_minfo("p:%d\n", p->getCount());
        XDLOG_minfo("t:%d\n", t->getCount());
    }
    XDSharedPtr<Test> tt(NULL);
    t = tt;
    if (t.isValid())
    XDLOG_minfo("t:%d\n", t->getCount());
    XDLOG_close();
    return 0;
}
