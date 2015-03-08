#include "event_linux.h"

XDSyncEventLinuxImp::XDSyncEventLinuxImp()
                   : cond_(&mutex_)
{
}

XDSyncEventLinuxImp::~XDSyncEventLinuxImp()
{
}
