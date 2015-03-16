#ifndef XD_SHARED_POINTER_H
#define XD_SHARED_POINTER_H

#include "types.h"

// 智能指针
// 只有继承XDShareable 或 实现XDIShareable接口的对象才能被智能指针管理
template <typename XDType>
class XDSharedPtr
{
public:
    explicit XDSharedPtr(XDType *ptr = NULL) : ptr_(ptr)
    {
        if (NULL != ptr_) {
            ptr_->inc();
        }
    }

    XDSharedPtr(const XDType &other) : ptr_(other.ptr_)
    {
        if (NULL != ptr_) {
            ptr_->inc();
        }
    }

    template <typename T>
    XDSharedPtr(const XDSharedPtr<T> &other) : ptr_(NULL)
    {
        ptr_ = other.get();
        if (NULL != ptr_) {
            ptr_->inc();
        }
    }

    ~XDSharedPtr()
    {
        if (NULL != ptr_) {
            if (0 == ptr_->dec()) {
                delete ptr_;
                ptr_ = NULL;
            }
        }
    }

    // T 必须为XDType的子类
    template <typename T>
    XDSharedPtr& operator = (const XDSharedPtr<T> &other)
    {
        if (ptr_ != other.get()) {
            if (NULL != other.get()) {
                other.get()->inc();
            }
            XDType *ptr = ptr_;
            ptr_ = other.get();
            if (NULL != ptr) {
                if (0 == ptr->dec()) {
                    delete ptr;
                    ptr = NULL;
                }
            }
        }
        return *this;
    }

    XDSharedPtr& operator = (const XDSharedPtr &other)
    {
        if (ptr_ != other.ptr_) {
            if (NULL != other.ptr_) {
                other.ptr_->inc();
            }
            XDType *ptr = ptr_;
            ptr_ = other.ptr_;
            if (NULL != ptr) {
                if (0 == ptr->dec()) {
                    delete ptr;
                    ptr = NULL;
                }
            }
        }
        return *this;
    }

    XDType* get() const
    {
        return ptr_;
    }

    XDType* operator -> () const
    {
        return ptr_;
    }

    XDType& operator * () const
    {
        return *ptr_;
    }

    bool isValid() const
    {
        return ptr_ ? true : false;
    }

private:
    XDType *ptr_;
};

#endif // end xd_shared_pointer_h
