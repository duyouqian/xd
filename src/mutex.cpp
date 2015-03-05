#include "mutex.h"
#include "error.h"

XDGuardMutex::XDGuardMutex(XDIMutex *mutex)
            : mutex_(mutex)
            , isLocked_(false)
{
    if (NULL != mutex_) {
        if (XDError::E_XD_SUCCESS == mutex_->lock()) {
            isLocked_ = true;
        }
    }
}

XDGuardMutex::~XDGuardMutex()
{
    if (NULL != mutex_) {
        if (XDError::E_XD_SUCCESS == mutex_->unlock()) {
            isLocked_ = false;
        }
    }
}

bool XDGuardMutex::isLocked() const
{
    return isLocked_;
}
