#include <stdio.h>
#include <string.h>
#include "timer_linux.h"
#include "error.h"

// 获取本地时间
struct tm* XDTimerLinuxImp::getLocalTime(const time_t* timer, struct tm* result)
{
    return ::localtime_r(timer, result);
}

uint64 XDTimerLinuxImp::now()
{
    return getCurTimestampByS();
}

uint64 XDTimerLinuxImp::getCurTimestampByS()
{
    struct ::timeval tv = {0, 0};
    getTimeOfDay(&tv, NULL);
    return (uint64)tv.tv_sec;
}

uint64 XDTimerLinuxImp::getCurTimestampByMS()
{
    struct ::timeval tv = {0, 0};
    getTimeOfDay(&tv, NULL);
    return (uint64)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

uint64 XDTimerLinuxImp::getCurTimestampByUS()
{
    struct ::timeval tv = {0, 0};
    getTimeOfDay(&tv, NULL);
    return (uint64)(tv.tv_sec * 1000000 + tv.tv_usec);
}

int32 XDTimerLinuxImp::getTimeOfDay(struct ::timeval *tv, struct timezone *tz)
{
    return ::gettimeofday(tv, tz);
}

bool XDTimerLinuxImp::isSameDayByTimestampWithS(uint64 fromSecond, uint64 toSecond)
{
    struct tm tm1, tm2;
    struct tm *pTm1 = NULL, *pTm2 = NULL;
    time_t t1 = (time_t)fromSecond;
    time_t t2 = (time_t)toSecond;
    pTm1 = getLocalTime(&t1, &tm1);
    if (NULL == pTm1) {
        return false;
    }
    pTm2 = getLocalTime(&t2, &tm2);
    if (NULL == pTm2) {
        return false;
    }
    tm1 = *pTm1;
    tm2 = *pTm2;
    int32 year1 = tm1.tm_year;
    int32 mon1 = tm1.tm_mon;
    int32 day1 = tm1.tm_mday;

    int32 year2 = tm2.tm_year;
    int32 mon2 = tm2.tm_mon;
    int32 day2 = tm2.tm_mday;

    if (year1 == year2 && mon1 == mon2 && day1 == day2) {
        return true;
    }
    return false;
}

void XDTimerLinuxImp::safeSleepByS(uint32 second)
{
    safeSleepByMS(second * 1000);
}

void XDTimerLinuxImp::safeSleepByMS(uint32 millisecond)
{
    struct ::timespec req, rem;
    req.tv_sec = millisecond / 1000U;
    req.tv_nsec = (uint64)((millisecond % 1000UL) * 1000UL * 1000UL);
    int32 ret = nanosleep(&req, &rem);
    while (EINTR == errno && -1 == ret) {
        ret = nanosleep(&req, &rem);
    }
}

XDErrorCode XDTimerLinuxImp::getAbsTimespec(struct timespec *ts, uint32 millisecond)
{
    struct timeval tv;
    int32 ret;
    if (NULL == ts) {
        return XDError::E_XD_INVAILD_PARAM;
    }
    ret = getTimeOfDay(&tv, NULL);
    if (0 != ret) {
        return ret;
    }
    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec * 1000UL;

    ts->tv_sec += millisecond / 1000UL;
    ts->tv_nsec += millisecond % 1000UL * 1000000UL;

    ts->tv_sec += ts->tv_nsec / (1000UL * 1000UL *1000UL);
    ts->tv_nsec %= (1000UL * 1000UL *1000UL);
    return XDError::E_XD_SUCCESS;
}


std::string XDTimerLinuxImp::getFormatTime(const char *fmt)
{
    struct ::timeval tv = {0, 0};
    getTimeOfDay(&tv, NULL);
    return getFormatTime(&tv, fmt);
}

std::string XDTimerLinuxImp::getFormatTime(uint64 timestamp, const char *fmt)
{
    struct ::timeval tv = {(time_t)timestamp, 0};
    return getFormatTime(&tv, fmt);
}

std::string XDTimerLinuxImp::getFormatTime(const struct ::timeval *tv, const char *fmt)
{
    struct tm rs;
    struct tm *pRs = NULL;
    time_t t = tv->tv_sec;
    char buffer[50] = {0};
    char tmp[8];
    pRs = getLocalTime(&t, &rs);
    if (NULL == pRs) {
        return NULL;
    }
    rs = *pRs;
    if (NULL == fmt) {
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &rs);
    } else {
        std::string fmtStr(fmt);
        std::string tmpStr;
        if (std::string::npos != fmtStr.find("[m]")) {
            snprintf(tmp, 8, "%03d", (uint32)(tv->tv_usec / 1000));
            tmpStr.assign(tmp);
            // fmtStr = StringUtil::replaceString(fmtString, "[m]", tmpStr);
        }
        strftime(buffer, sizeof(buffer), fmtStr.c_str(), &rs);
    }
    return std::string(buffer);
}

// 时间差
void XDTimerLinuxImp::subtratTimeval(const struct timeval *from, struct timeval *sub, struct timeval *rs)
{
    if (from->tv_usec < sub->tv_usec) {
        int32 n = ((sub->tv_usec - from->tv_usec) / 1000000) + 1;
        sub->tv_usec -= 1000000 * n;
        sub->tv_sec += n;
    }
    if ((from->tv_usec - sub->tv_usec) > 1000000) {
        int32 m = (from->tv_usec - sub->tv_usec) / 1000000;
        sub->tv_usec += m * 1000000;
        sub->tv_sec -= m;
    }
    rs->tv_sec = from->tv_sec - sub->tv_sec;
    rs->tv_usec = from->tv_usec - sub->tv_usec;
}
