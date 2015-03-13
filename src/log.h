#ifndef XD_LOG_H
#define XD_LOG_H

#include "noncopyable.h"
#include "file.h"
#include "types.h"
#include "thread.h"
#include "event.h"

#define XDLOG_LEVEL_FATAL 5
#define XDLOG_LEVEL_ERROR 4
#define XDLOG_LEVEL_WARN  3
#define XDLOG_LEVEL_INFO  2
#define XDLOG_LEVEL_DEBUG 1

#define MAX_LOG_CONTEXT 1024
#define MAX_LOG_LOCATE 256
#define MAX_LOG_THREADINFO 32
#define MAX_LOG_RECODE 2048

#define XDLOG_open(path, lv) gLog.open((path), (lv))
#define XDLOG_close() gLog.close()

#define XDLOG_mfatal(format, ...) gLog.write(XDLOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define XDLOG_merror(format, ...) gLog.write(XDLOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define XDLOG_mwarn(format, ...) gLog.write(XDLOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define XDLOG_minfo(format, ...) gLog.write(XDLOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define XDLOG_mdebug(format, ...) gLog.write(XDLOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

struct XDLogMsgItem
{
    uint8 lv;
    uint32 threadID;
    uint64 timestamp;
    char locate[MAX_LOG_LOCATE];
    char context[MAX_LOG_CONTEXT];
};

class XDLog : public XDIRunnable
{
public:
    XDLog();
    ~XDLog();
    bool open(const char *path, uint8 outputLv);
    bool close();
    bool write(uint8 lv, const char *file, int line, const char *func, const char *format, ...);
    bool exec(XDLogMsgItem *item);
    uint32 run();
private:
    bool createFile();
private:
    XDFile file_;
    char path_[MAX_PATH_LEN];
    uint8 outputLv_;
    volatile bool isInit_;
    uint64 lastTouch_;
    XDThread *thread_;
    XDSyncEvent *event_;
};

extern XDLog gLog;

#endif // end xd_log_h
