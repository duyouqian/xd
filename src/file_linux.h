#ifndef XD_FILEIMP_LINUX_H
#define XD_FILEIMP_LINUX_H

#include "base_file.h"

class XDFileHandleLinuxImp : public XDIFileHandle
{
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
private:
    FORCEINLINE bool isValid()
    {
        return -1 != fileHandle_;
    }
private:
    const static uint32 READWRITE_SIZE;
    int32 fileHandle_;
};

#endif // end xd_fileimp_linux_h
