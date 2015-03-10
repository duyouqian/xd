#include "file_linux.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

const uint32 XDFileHandleLinuxImp::READWRITE_SIZE = 1024 * 1024;

XDFileHandleLinuxImp::XDFileHandleLinuxImp(int32 fileHandle)
                    : fileHandle_(fileHandle)
{
    check(fileHandle_ > -1);
}

XDFileHandleLinuxImp::~XDFileHandleLinuxImp()
{
    close(fileHandle_);
    fileHandle_ = false;
}

int64 XDFileHandleLinuxImp::tell()
{
    check(isValid());
    return lseek(fileHandle_, 0, SEEK_CUR);
}

bool XDFileHandleLinuxImp::seek(int64 newPostition)
{
    check(newPostition >= 0);
    check(isValid());
    return -1 != lseek(fileHandle_, newPostition, SEEK_SET);
}

bool XDFileHandleLinuxImp::seekFromEnd()
{
    check(isValid());
    return -1 != lseek(fileHandle_, 0, SEEK_END);
}

bool XDFileHandleLinuxImp::read(uint8 *buffer, uint32 len)
{
    check(isValid());
    int64 bytesRead = 0;
    while (len) {
        check(len >= 0);
        int64 curLen = MIN(READWRITE_SIZE, len);
        check(buffer);
        int64 curReadLen = ::read(fileHandle_, buffer, curLen);
        bytesRead += curReadLen;
        if (curReadLen != curLen) {
            break;
        }
        buffer += curLen;
        len -= curLen;
    }
    return len == bytesRead;
}

bool XDFileHandleLinuxImp::write(const uint8 *msg, uint32 len)
{
    check(isValid());
    while(len) {
        check(len >= 0);
        int64 curLen = MIN(READWRITE_SIZE, len);
        check(msg);
        int64 writeLen = ::write(fileHandle_, msg, curLen);
        if (writeLen != curLen) {
            return false;
        }
        msg += curLen;
        len -= curLen;
    }
    return true;
}

uint64 XDFileHandleLinuxImp::size()
{
    check(isValid());
    struct stat fileInfo;
    fstat(fileHandle_, &fileInfo);
    return fileInfo.st_size;
}
