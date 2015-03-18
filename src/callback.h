#ifndef XD_CALLBACK_H
#define XD_CALLBACK_H

#include "shareable.h"
#include "shared_pointer.h"

class XDIOEventReadCallBack : public XDShareable
{
public:
    virtual bool exec()
    {
        return true;
    }
};

typedef XDSharedPtr<XDIOEventReadCallBack> XDIOEventReadCallBackPtr;

#endif // end xd_callback_h
