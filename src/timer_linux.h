#ifndef XD_TIMERIMP_LINUX_H
#define XD_TIMERIMP_LINUX_H

#include <time.h>
#include <sys/time.h>
#include <string>

#include "noncopyable.h"
#include "types.h"

class XDTimerLinuxImp : public XDNoncopyable
{
public:
    // 获取本地时间
    static struct tm* getLocalTime(const time_t* timer, struct tm* result);
    // 获取当前时间戳 s(秒)
    static uint64 getCurTimestampByS();
    // 获取当前时间戳 ms(毫秒)
    static uint64 getCurTimestampByMS();
    // 获取当前时间戳 us(微妙)
    static uint64 getCurTimestampByUS();
    // 获取时间
    static int32 getTimeOfDay(struct ::timeval *tv, struct timezone *tz);
    // 安全休眠 s(秒)
    static void safeSleepByS(uint32 second);
    // 安全休眠 ms(毫秒)
    static void safeSleepByMS(uint32 millisecond);
    // 获取绝对时间
    static XDErrorCode getAbsTimespec(struct ::timespec *ts, uint32 millisecond);
    // 格式化时间
    static std::string getFormatTime(const char *fmt = NULL);
    static std::string getFormatTime(const struct ::timeval *tv,const char *fmt = NULL);
};

#endif // end xd_timerimp_linux_h
