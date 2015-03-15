#include "shareable.h"

XDShareable::~XDShareable()
{
}

void XDShareable::inc()
{
    ref_.increment();
}

int32 XDShareable::getCount()
{
    return ref_.getValue();
}

int32 XDShareable::dec()
{
    if (ref_.getValue() > 0) {
        ref_.decrement();
    }
    return ref_.getValue();
}
