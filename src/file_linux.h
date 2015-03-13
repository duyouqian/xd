#ifndef XD_FILEIMP_LINUX_H
#define XD_FILEIMP_LINUX_H

#include "base_file.h"

class XDFileLinuxImp;

class XDFileHandleLinuxImp : public XDIFileHandle
{
    friend class XDFileLinuxImp;
public:
    explicit XDFileHandleLinuxImp(int32 fileHandle);
    ~XDFileHandleLinuxImp();
    // 获取当前读写位置
    int64 tell();
    // 设置读写位置
    bool seek(int64 newPosition);
    // 设置读写位置至文件末尾
    bool seekFromEnd();
    // 读取文件
    bool read(uint8 *buffer, uint32 len);
    // 写文件
    bool write(const uint8 *msg, uint32 len);
    // 文件大小
    uint64 size();
    // 改变文件句柄
    void changeFileHandle(int32 newFileHandle);
    // 关闭文件
    void close();
private:
    FORCEINLINE bool isValid()
    {
        return -1 != fileHandle_;
    }
private:
    const static uint32 READWRITE_SIZE;
    int32 fileHandle_;
};

class XDFileLinuxImp : public XDIFile
{
public:
    XDFileLinuxImp();
    ~XDFileLinuxImp();
    // 只读打开文件
    bool readOpen(const char *name);
    // 只写打开文件
    bool writeOpen(const char *name, bool isAppend, bool isAllowRead);
    // 关闭文件
    bool close();
    // 读取文件
    bool read(uint8 *buffer, uint32 len);
    // 写文件
    bool write(const uint8 *msg, uint32 len);
private:
    FORCEINLINE bool isValid()
    {
        return handle_ ? handle_->isValid() : false;
    }
private:
    XDFileHandleLinuxImp *handle_;
};

#endif // end xd_fileimp_linux_h
