#ifndef XD_BASE_FILE_H
#define XD_BASE_FILE_H

#include "noncopyable.h"
#include "types.h"

class XDIFileHandle : public XDNoncopyable
{
public:
    // 读取文件
    virtual bool read(uint8 *buffer, uint32 len) = 0;
    // 写文件
    virtual bool write(const uint8 *msg, uint32 len) = 0;
};

class XDIFile : public XDNoncopyable
{
public:
    // 打开文件
    virtual bool open(const char *name, const char *type) = 0;
    // 关闭文件
    virtual bool close() = 0;
};

#endif // end xd_base_file_h
