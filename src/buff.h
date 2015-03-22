#ifndef XD_BUFFER_H
#define XD_BUFFER_H

#include "types.h"
#include <vector>

class XDBuffer
{
public:
private:
    // buffer
    std::vector<char> buffer_;
    // 可读索引
    uint32 readerIndex_;
    // 可写索引
    uint32 writenIndex_;
};

#endif // end xd_buffer_h
