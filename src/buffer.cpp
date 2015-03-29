#include "buffer.h"
#include "socket_util.h"
#include <string.h>
#include <algorithm>
#include <sys/uio.h>

const char XDBuffer::CRLF[] = "\r\n";

XDBuffer::XDBuffer(uint32 initSize)
        : buffer_(CheapIndex + initSize)
        , writeIndex_(CheapIndex)
        , readIndex_(CheapIndex)
{
    check(readableBytes() == 0);
    check(writableBytes() == initSize);
    check(prependableBytes() == CheapIndex);
}

void XDBuffer::swap(XDBuffer &other)
{
    buffer_.swap(other.buffer_);
    std::swap(readIndex_, other.readIndex_);
    std::swap(writeIndex_, other.writeIndex_);
}

uint32 XDBuffer::readableBytes() const
{
    return writeIndex_ - readIndex_;
}

uint32 XDBuffer::writableBytes() const
{
    return buffer_.size() - writeIndex_;
}

uint32 XDBuffer::prependableBytes() const
{
    return readIndex_;
}

const char* XDBuffer::peek() const
{
    return begin() + readIndex_;
}

const char* XDBuffer::findCRLF() const
{
    const char *crlf = std::search(peek(), beginWrite(), CRLF, CRLF + 2);
    return crlf == beginWrite() ? NULL : crlf;
}

const char* XDBuffer::findCRLF(const char *start) const
{
    check(peek() <= start);
    check(start <= beginWrite());
    const char *crlf = std::search(start, beginWrite(), CRLF, CRLF + 2);
    return crlf == beginWrite() ? NULL : crlf;
}

const char* XDBuffer::findEOL() const
{
    const void *eol = memchr(peek(), '\n', readableBytes());
    return static_cast<const char*>(eol);
}

const char* XDBuffer::findEOL(const char *start) const
{
    check(peek() <= start);
    check(start <= beginWrite());
    const void *eol = memchr(start, '\n', readableBytes());
    return static_cast<const char*>(eol);
}

void XDBuffer::retrieve(uint32 len)
{
    check(len <= readableBytes());
    if (len < readableBytes()) {
        readIndex_ += len;
    } else {
        retrieveAll();
    }
}

void XDBuffer::retrieveUntil(const char *end)
{
    check(peek() <= end);
    check(end <= beginWrite());
    retrieve(end - peek());
}

void XDBuffer::retrieveAll()
{
    readIndex_ = CheapIndex;
    writeIndex_ = CheapIndex;
}

std::string XDBuffer::retrieveAsString(uint32 len)
{
    check(len <= readableBytes());
    std::string rs(peek(), len);
    retrieve(len);
    return rs;
}

void XDBuffer::append(const char *data, uint32 len)
{
    ensureWritableBytes(len);
    std::copy(data, data + len, beginWrite());
    hasWrite(len);
}

void XDBuffer::ensureWritableBytes(uint32 len)
{
    if (writableBytes() < len) {
        makeSpace(len);
    }
    check(writableBytes() >= len);
}

char* XDBuffer::beginWrite()
{
    return begin() + writeIndex_;
}

const char* XDBuffer::beginWrite() const
{
    return begin() + writeIndex_;
}

void XDBuffer::hasWrite(uint32 len)
{
    check(len <= writableBytes());
    writeIndex_ += len;
}

void XDBuffer::unWrite(uint32 len)
{
    check(len <= readableBytes());
    writeIndex_ -= len;
}

char* XDBuffer::begin()
{
    return &*buffer_.begin();
}

const char* XDBuffer::begin() const
{
    return &*buffer_.begin();
}

void XDBuffer::makeSpace(uint32 len)
{
    if (writableBytes() + prependableBytes() < len + CheapIndex) {
        buffer_.resize(len + writeIndex_);
    } else {
        check(CheapIndex < readIndex_);
        uint32 readable = readableBytes();
        std::copy(begin() + readIndex_,
                  begin() + writeIndex_,
                  begin() + CheapIndex);
        readIndex_ = CheapIndex;
        writeIndex_ = readIndex_ + readable;
        check(readable == readableBytes());
    }
}

int32 XDBuffer::readFD(FD fd, int *saveErrno)
{
    char extrabuf[65535];
    struct iovec vec[2];
    const int32 writable = writableBytes();
    vec[0].iov_base = begin() + writeIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);
    const int32 iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    const int32 n = XDSocketOpt::readv(fd, vec, iovcnt);
    if (-1 == n) {
        *saveErrno = errno;
    } else if (n <= writable) {
        writeIndex_ += n;
    } else {
        writeIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;
}
