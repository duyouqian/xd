#include "reference_count.h"

XDRefCounter::XDRefCounter()
            : counter_(0)
{
}

XDRefCounter::XDRefCounter(int32 value)
            : counter_(value)
{
}

XDRefCounter::~XDRefCounter()
{
}

int32 XDRefCounter::increment()
{
    return XDAtomics::interlockedIncrement(&counter_);
}

int32 XDRefCounter::decrement()
{
    return XDAtomics::interlockedDecrement(&counter_);
}

int32 XDRefCounter::add(int32 amount)
{
    return XDAtomics::interlockedAdd(&counter_, amount);
}

int32 XDRefCounter::subtrat(int32 amount)
{
    return XDAtomics::interlockedAdd(&counter_, -amount);
}

int32 XDRefCounter::set(int32 amount)
{
    return XDAtomics::interlockedExchange(&counter_, amount);
}

int32 XDRefCounter::reset()
{
    return XDAtomics::interlockedExchange(&counter_, 0);
}

int32 XDRefCounter::getValue() const
{
    return counter_;
}
