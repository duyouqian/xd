//
//  singleton.h
//  xd
//
//  Created by duyouqian on 15-2-20.
//  Copyright (c) 2015年 duyouqian. All rights reserved.
//

#ifndef XD_SINGLETON_H
#define XD_SINGLETON_H

#include <pthread.h>
#include <stdlib.h>

// 单例工具 在linux下工作
template <typename Type>
class XDSingleton
{
public:
    static Type& getInstance()
    {
        pthread_once(&once_, &XDSingleton::init);
        return *instance_;
    }
    static void init()
    {
        instance_ = new (Type)();
        atexit(destroy);
    }
    static bool isVaild()
    {
        return NULL != instance_;
    }
    
protected:
    XDSingleton() { }
    ~XDSingleton() { }
    
private:
    static void destroy()
    {
        if (NULL != instance_) {
            delete instance_;
            instance_ = NULL;
        }
    }
    static pthread_once_t once_;
    static Type* volatile instance_;
};

template <typename Type>
pthread_once_t XDSingleton<Type>::once_ = PTHREAD_ONCE_INIT;
template <typename Type>
Type* volatile XDSingleton<Type>::instance_ = NULL;

#endif // end xd_singleton_h
