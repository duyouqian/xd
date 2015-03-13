#include "log.h"
#include "timer.h"
#include "thread.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <string>

static const char* constBaseName(const char *filePath)
{
    const char *base = strrchr(filePath, '/');
    return base ? base + 1 : filePath;
}

XDLog gLog;

XDLog::XDLog()
     : outputLv_(0)
     , isInit_(false)
     , lastTouch_(0)
     , thread_(NULL)
     , event_(NULL)
{
    bzero(path_, sizeof(path_));
}

XDLog::~XDLog()
{
}

bool XDLog::open(const char *path, uint8 outputLv)
{
    if (isInit_) {
        fprintf(stderr, "已经初始化过了\n");
        return false;
    }
    if (strlen(path) > MAX_PATH_LEN) {
        fprintf(stderr, "路径名太长,当前长度:%lu, 最大长度:%d\n", strlen(path), MAX_PATH_LEN);
        return false;
    }
    strncpy(path_, path, MAX_PATH_LEN);
    outputLv_ = outputLv;
    isInit_ = createFile();
    return isInit_;
}

bool XDLog::close()
{
    isInit_ = false;
    return file_.close();
}

bool XDLog::write(uint8 lv, const char *file, int line, const char *func, const char *format, ...)
{
    if (!isInit_) {
        fprintf(stderr, "XDLog还没初始化\n");
        return false;
    }
    if (lv < outputLv_) {
        return false;
    }
    struct XDLogMsgItem item;
    bzero(item.locate, sizeof(item.locate));
    bzero(item.context, sizeof(item.context));
    item.timestamp = XDTimer::now();
    item.lv = lv;
    item.threadID = XDThread::getCurrentThreadID();
    snprintf(item.locate, MAX_LOG_LOCATE, "%s:%d:%s", file, line, func);
    // 构建内容
    va_list va;
    va_start(va, format);
    vsnprintf(item.context, MAX_LOG_CONTEXT, format, va);
    va_end(va);
    exec(&item);
    return true;
}

bool XDLog::createFile()
{
    file_.close();
    uint64 curTimestamp = XDTimer::getCurTimestampByS();
    std::string timeFmt = XDTimer::getFormatTime("_%Y_%m_%d.log");
    std::string fileFullName(path_);
    fileFullName.append(timeFmt);
    bool created = file_.writeOpen(fileFullName.c_str(), true, false);
    if (created) {
        lastTouch_ = curTimestamp;
    } else {
        fprintf(stderr, "创建文件失败,文件全名:%s", fileFullName.c_str());
    }
    return created;
}

bool XDLog::exec(XDLogMsgItem *item)
{
    check(item);
    if (!XDTimer::isSameDayByTimestampWithS(lastTouch_, XDTimer::now())) {
        createFile();
    }
    const char *lvColor = NULL;
    const char *lvText = NULL;
    const char *clr = "\033[0m";
    switch(item->lv) {
        case XDLOG_LEVEL_FATAL:
            lvColor = "\033[35m";
            lvText = "[fatal]";
        break;
        case XDLOG_LEVEL_ERROR:
            lvColor = "\033[31m";
            lvText = "[error]";
        break;
        case XDLOG_LEVEL_WARN:
            lvColor = "\033[36m";
            lvText = "[warn ]";
        break;
        case XDLOG_LEVEL_INFO:
            lvColor = "\033[33m";
            lvText = "[info ]";
        break;
        case XDLOG_LEVEL_DEBUG:
            lvColor = "\033[34m";
            lvText = "[debug]";
        break;
        default:
            lvColor = "\033[33m";
            lvText = "[info ]";
        break;
    }
    std::string timestamp = XDTimer::getFormatTime(item->timestamp, "%H:%M:%S");
    char threadInfo[MAX_LOG_THREADINFO];
    snprintf(threadInfo, MAX_LOG_THREADINFO, "[tid=%u]", item->threadID);
    char buffer[MAX_LOG_RECODE];
    // 写终端
    bzero(buffer, sizeof(buffer));
    snprintf(buffer, MAX_LOG_RECODE, "%s%s[%s]%s%s[%s]%s%s\n", clr, lvColor, timestamp.c_str(), lvText, threadInfo, constBaseName(item->locate), item->context, clr);
    printf("%s", buffer);
    // 写文件
    bzero(buffer, sizeof(buffer));
    snprintf(buffer, MAX_LOG_RECODE, "[%s]%s%s[%s]%s\n", timestamp.c_str(), lvText, threadInfo, constBaseName(item->locate), item->context);
    file_.write((const uint8*)buffer, strlen(buffer));
}

uint32 XDLog::run()
{
}
