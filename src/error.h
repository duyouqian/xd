#ifndef XD_ERROR_H
#define XD_ERROR_H

namespace XDError
{
    enum XDERROR
    {
        E_XD_SUCCESS,               // 成功
        E_XD_NULL_POINTER = -10000, // 空指针
        E_XD_CONF_ERROR,            // 配置错误
        E_XD_NOMEM,                 // 内存不足
        E_XD_INVAILD_PARAM,         // 无效参数
        E_XD_SYSERROR,           // 系统错误
        E_XD_TIMEOUT,               // 超时
        E_XD_FAILURE,               // 失败
    };
}; // end namespace XDError

#endif // end xd_error_h
