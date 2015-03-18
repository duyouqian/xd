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

class Test3
{
public:
    Test3(int32 t1)
    {
        t1_ = t1;
    }
private:
    int32 t1_;
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
        //XDSharedPtr<Test> tmp(NULL);
        //t = tmp;
    }
    t.setNull();

    Test3* t3 = (Test3*)malloc(sizeof(Test3));
    new (t3)Test3(10);
    free(t3);
    XDLOG_close();
    return 0;
}
