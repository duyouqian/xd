#include "thread_group.h"
#include <stdio.h>

XDThreadGroup::XDThreadGroup(const char *name)
             : name_(name)
{
}

XDThreadGroup::~XDThreadGroup()
{
    std::map<uint32, XDIThread*>::iterator it;
    for (it = allThreads_.registry_.begin(); it != allThreads_.registry_.end(); ++it) {
        delete it->second;
    }
    allThreads_.registry_.clear();
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

void XDThreadGroup::waitForComplateionAll()
{
    std::map<uint32, XDIThread*>::iterator it;
    for (it = allThreads_.registry_.begin(); it != allThreads_.registry_.end(); ++it) {
        (it->second)->waitForComplateion();
    }
}
