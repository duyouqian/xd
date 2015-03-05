#ifndef XD_REFERENCECOUNT_H
#define XD_REFERENCECOUNT_H

#include "noncopyable.h"
#include "atomics.h"
#include "types.h"

class XDRefCounter : public XDNoncopyable
{
public:
    XDRefCounter();
    explicit XDRefCounter(int32 value);
    ~XDRefCounter();
    // ++count
    // return new value
    int32 increment();
    // --count
    // return new value
    int32 decrement();

    // count = count + value
    // retrun old value
    int32 add(int32 amount);

    // count = count - value
    // return old value
    int32 subtrat(int32 amount);

    // count = value
    // return old value
    int32 set(int32 amount);

    // reset zero
    // count = 0
    // return old value
    int32 reset();

    // get current value
    int32 getValue() const;

private:
    volatile int32 counter_;
};

#endif // end xd_referencecount_h
