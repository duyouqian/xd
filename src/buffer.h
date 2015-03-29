#ifndef XD_BUFFER_H
#define XD_BUFFER_H

#include "noncopyable.h"
#include "types.h"
#include <string>
#include <vector>

class XDBuffer : public XDNoncopyable
{
public:
    static const uint32 CheapIndex = 8;
    static const uint32 InitSize = 1024;
public:
    explicit XDBuffer(uint32 initSize = InitSize);
    void swap(XDBuffer &other);
    // 可读数
    uint32 readableBytes() const;
    // 可写数
    uint32 writableBytes() const;
    // 可读起始位
    uint32 prependableBytes() const;

    const char* peek() const;
    const char* findCRLF() const;
    const char* findCRLF(const char *start) const;
    const char* findEOL() const;
    const char* findEOL(const char *start) const;

    void retrieve(uint32 len);
    void retrieveUntil(const char *end);
    void retrieveInt64() { retrieve(sizeof(int64)); }
    void retrieveInt32() { retrieve(sizeof(int32)); }
    void retrieveInt16() { retrieve(sizeof(int16)); }
    void retrieveInt8()  { retrieve(sizeof(int8));  }
    void retrieveAll();
    std::string retrieveAllAsString() { return retrieveAsString(readableBytes()); }
    std::string retrieveAsString(uint32 len);
    void append(const char *data, uint32 len);
    void append(const void *data, uint32 len) { append(static_cast<const char*>(data), len); }

    void ensureWritableBytes(uint32 len);

    char *beginWrite();
    const char *beginWrite() const;
    void hasWrite(uint32 len);
    void unWrite(uint32 len);

    int32 readFD(FD fd, int32 *saveErrno);
private:
    char *begin();
    const char* begin() const;
    void makeSpace(uint32 len);
private:
    std::vector<char> buffer_;
    uint32 writeIndex_;
    uint32 readIndex_;
    static const char CRLF[];
};

#endif // end xd_buffer_h
