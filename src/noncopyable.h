#ifndef XD_NONCOPYABLE_H
#define XD_NONCOPYABLE_H

// 禁止拷贝
class XDNoncopyable
{
public:
    XDNoncopyable() {}
    virtual ~XDNoncopyable() {}
private:
    XDNoncopyable(const XDNoncopyable&);
    const XDNoncopyable& operator = (const XDNoncopyable&);
};

#endif // end xd_noncopyable_h
