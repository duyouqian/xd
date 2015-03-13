#include "file_linux.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

const uint32 XDFileHandleLinuxImp::READWRITE_SIZE = 1024 * 1024;

XDFileHandleLinuxImp::XDFileHandleLinuxImp(int32 fileHandle)
                    : fileHandle_(fileHandle)
{
    check(isValid());
}

XDFileHandleLinuxImp::~XDFileHandleLinuxImp()
{
    close();
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

void XDFileHandleLinuxImp::changeFileHandle(int32 newFileHandle)
{
    close();
    fileHandle_ = newFileHandle;
    check(isValid());
}

void XDFileHandleLinuxImp::close()
{
    if (isValid()) {
        ::close(fileHandle_);
    }
    fileHandle_ = -1;
}

XDFileLinuxImp::XDFileLinuxImp()
              : handle_(NULL)
{
}

XDFileLinuxImp::~XDFileLinuxImp()
{
    if (handle_) {
        delete handle_;
    }
    handle_ = NULL;
}

bool XDFileLinuxImp::readOpen(const char *name)
{
    //
    int32 fileHandle = open(name, O_RDONLY);
    if (-1 == fileHandle) {
        return false;
    }
    if (handle_) {
        handle_->changeFileHandle(fileHandle);
    } else {
        handle_ = new XDFileHandleLinuxImp(fileHandle);
    }
    return true;
}

bool XDFileLinuxImp::writeOpen(const char *name, bool isAppend, bool isAllowRead)
{
    int32 flags = O_CREAT | O_CLOEXEC;
    if (isAppend) {
        flags |= O_APPEND;
    }
    if (isAllowRead) {
        flags |= O_RDWR;
    } else {
        flags |= O_WRONLY;
    }
    int32 fileHandle = ::open(name, flags, S_IRUSR | S_IWUSR);
    if (-1 == fileHandle) {
        return false;
    }
    if (handle_) {
        handle_->changeFileHandle(fileHandle);
    } else {
        handle_ = new XDFileHandleLinuxImp(fileHandle);
    }
    return true;
}

bool XDFileLinuxImp::close()
{
    if (handle_) {
        handle_->close();
        return true;
    }
    return false;
}

bool XDFileLinuxImp::read(uint8 *buffer, uint32 len)
{
    check(isValid());
    return handle_->read(buffer, len);
}

bool XDFileLinuxImp::write(const uint8 *msg, uint32 len)
{
    check(isValid());
    return handle_->write(msg, len);
}
