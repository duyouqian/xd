#include "common.h"

class Test : public XDShareable
{
public:
    void TestImp()
    {
        XDLOG_minfo("test1");
    }
    virtual ~Test()
    {
        XDLOG_minfo("Test destroy");
    }
};

class Test2 : public Test
{
public:
    void Test2Imp()
    {
        XDLOG_minfo("test2");
    }
    ~Test2()
    {
        XDLOG_minfo("Test2 destroy");
    }

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
    XDLOG_minfo("t:%d\n", t->getCount());
    {
        XDSharedPtr<Test2> tt(new Test2());
        XDLOG_minfo("tt:%d\n", tt->getCount());
        t = tt;
    }
    if (t.isValid())
        XDLOG_minfo("t:%d\n", t->getCount());
    {
        XDSharedPtr<Test> tmp(NULL);
        t = tmp;
    }
    XDLOG_close();
    return 0;
}
