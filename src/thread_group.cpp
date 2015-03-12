#include "thread_group.h"
#include <stdio.h>

XDThreadGroup::XDThreadGroup(const char *name)
             : name_(name)
{
}

XDThreadGroup::~XDThreadGroup()
{
}

void XDThreadGroup::addThread(uint32 id, XDIThread *thread)
{
    allThreads_.add(id, thread);
}

void XDThreadGroup::removeThread(uint32 id)
{
    allThreads_.remove(id);
}

XDIThread* XDThreadGroup::getThreadByID(uint32 id)
{
    return allThreads_.getThread(id);
}

uint32 XDThreadGroup::getCount()
{
    return allThreads_.getCount();
}
