#include "common.h"

class T2;

class T1 : public XDShareable
{
public:
    void test(XDSharedPtr<T2> &t2)
    {
        XDLOG_minfo("");
    }
};

class T2 : public XDShareable
{
public:
    void setT1(XDSharedPtr<T1> &t1)
    {
        t1_ = t1;
    }
    void test()
    {
        XDSharedPtr<T2> t2(new T2);
        t1_->test(t2);
    }
private:
    XDSharedPtr<T1> t1_;
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDSharedPtr<T1> t1(new T1());
    {
        T2 t2;
    t2.setT1(t1);
    t2.test();
    }
    XDLOG_close();
    return 0;
}
