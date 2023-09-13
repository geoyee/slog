/*
 @ brief:   A simple log system for C++14
 @ author:  Yizhou Chen
 @ date:    2023-09-13
 @ version: 1.0.0
 */

#ifndef _SLOG_H_
#define _SLOG_H_

#include <iostream>
#include <string>
#include <functional>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cassert>
#include <type_traits>
#include <utility>

typedef enum
{
    CE_None = 0,
    CE_Debug = 1,
    CE_Warning = 2,
    CE_Failure = 3,
    CE_Fatal = 4
} CPLErr;

typedef enum
{
    CPLE_None,
    CPLE_AppDefined,
    CPLE_OutOfMemory,
    CPLE_FileIO,
    CPLE_OpenFailed,
    CPLE_IllegalArg,
    CPLE_NotSupported,
    CPLE_AssertionFailed,
    CPLE_NoWriteAccess,
    CPLE_UserInterrupt,
    CPLE_ObjectNull,
    CPLE_HttpResponse,
    CPLE_AWSBucketNotFound,
    CPLE_AWSObjectNotFound,
    CPLE_AWSAccessDenied,
    CPLE_AWSInvalidCredentials,
    CPLE_AWSSignatureDoesNotMatch,
} CPLErrorNum;

#ifdef CPL_ERROR_H_INCLUDED
#include <cpl_error.h>
#define SINFO CPLError
#else
#define SINFO(eErrClass, err_no, ...) \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
        if (eErrClass == CE_Fatal)    \
            exit(1);                  \
    }                                 \
    while (0)
#endif

std::string nowTimeStr()
{
    std::time_t t = std::time(nullptr);
    char buffer[20];
    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y/%m/%d %H:%M:%S",
        std::localtime(&t));
    return std::string(buffer);
}

// `if constexpr` is a C++17 feature, so use `std::enable_if_t` instead

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 0, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj);
}

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 1, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj, std::placeholders::_1);
}

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 2, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj, std::placeholders::_1,
                     std::placeholders::_2);
}

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 3, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj, std::placeholders::_1,
                     std::placeholders::_2,
                     std::placeholders::_3);
}

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 4, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj, std::placeholders::_1,
                     std::placeholders::_2,
                     std::placeholders::_3,
                     std::placeholders::_4);
}

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 5, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj, std::placeholders::_1,
                     std::placeholders::_2,
                     std::placeholders::_3,
                     std::placeholders::_4,
                     std::placeholders::_5);
}

template <typename CLS, typename RET, typename... ARGS, std::enable_if_t<sizeof...(ARGS) == 6, int> = 1>
std::function<RET(ARGS...)> makePlaceholders(RET (CLS::*func)(ARGS...), CLS *obj)
{
    return std::bind(func, obj, std::placeholders::_1,
                     std::placeholders::_2,
                     std::placeholders::_3,
                     std::placeholders::_4,
                     std::placeholders::_5,
                     std::placeholders::_6);
}

template <typename RET, typename... ARGS, std::enable_if_t<!std::is_same<RET, void>::value, int> = 1>
RET runFunction(std::function<RET(ARGS...)> func, ARGS... args)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    RET result = func(args...);
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    double ms = duration.count() * 1000.0;
    SINFO(CE_Debug, CPLE_None, "  [Success]\tIt takes %lf ms", ms);
    return result;
}

template <typename RET, typename... ARGS, std::enable_if_t<std::is_same<RET, void>::value, int> = 1>
RET runFunction(std::function<RET(ARGS...)> func, ARGS... args)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    func(args...);
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    double ms = duration.count() * 1000.0;
    SINFO(CE_Debug, CPLE_None, "  [Success]\tIt takes %lf ms", ms);
    return RET();
}

template <typename>
class TimeLog;

template <typename RET, typename... ARGS>
class TimeLog<RET(ARGS...)>
{
private:
    std::function<RET(ARGS...)> _func;
    std::string _func_name;
    std::string _file_name;
    std::string _args_name;
    int _line_no;

public:
    constexpr TimeLog(std::function<RET(ARGS...)> func,
                      const char *func_name,
                      const char *file_name,
                      const char *args_name,
                      int line_no)
        : _func(func),
          _func_name(func_name),
          _file_name(file_name),
          _args_name(args_name),
          _line_no(line_no)
    {
    }
    template <typename CLS>
    constexpr TimeLog(RET (CLS::*func)(ARGS...),
                      CLS *obj,
                      const char *func_name,
                      const char *file_name,
                      const char *args_name,
                      int line_no)
        : _func_name(func_name),
          _file_name(file_name),
          _args_name(args_name),
          _line_no(line_no)
    {
        _func = makePlaceholders<CLS, RET, ARGS...>(func, obj);
    }
    RET operator()(ARGS... args)
    {
        SINFO(CE_Debug, CPLE_None, "~ %s", nowTimeStr().c_str());
        SINFO(CE_Debug, CPLE_None, "  [Function]\t%s(%s)", _func_name.c_str(), _args_name.c_str());
        SINFO(CE_Debug, CPLE_None, "  [Location]\t%s (%d)", _file_name.c_str(), _line_no);
        try
        {
            return runFunction<RET, ARGS...>(_func, args...);
        }
        catch (const std::exception &ex)
        {
            SINFO(CE_Failure, CPLE_AppDefined, "  [Failure]\t%s", ex.what());
            return RET();
        }
        catch (...)
        {
            SINFO(CE_Fatal, CPLE_AppDefined, "  [Fatal]\tUnknown exception");
            return RET();
        }
    }
};

template <typename RET, typename... ARGS>
TimeLog<RET(ARGS...)> makeTimeLogFunction(RET (*func)(ARGS...),
                                          const char *func_name,
                                          const char *file_name,
                                          const char *args_name,
                                          int line_no)
{
    return TimeLog<RET(ARGS...)>(std::function<RET(ARGS...)>(func),
                                 func_name,
                                 file_name,
                                 args_name,
                                 line_no);
}

template <typename RET, typename CLS, typename... ARGS>
TimeLog<RET(ARGS...)> makeTimeLogClassFunction(RET (CLS::*func)(ARGS...),
                                               CLS *obj,
                                               const char *func_name,
                                               const char *file_name,
                                               const char *args_name,
                                               int line_no)
{
    return TimeLog<RET(ARGS...)>(func,
                                 obj,
                                 func_name,
                                 file_name,
                                 args_name,
                                 line_no);
}

#ifdef _SLOG_DEBUG
#define SSF                                                                  \
    SINFO(CE_Debug, CPLE_None, "- %s", nowTimeStr().c_str());                \
    SINFO(CE_Debug, CPLE_None, "  [Function]\t%s", __FUNCTION__);            \
    SINFO(CE_Debug, CPLE_None, "  [Location]\t%s (%d)", __FILE__, __LINE__); \
    try                                                                      \
    {

#define SEF(result)                                                       \
    }                                                                     \
    catch (const std::exception &ex)                                      \
    {                                                                     \
        SINFO(CE_Failure, CPLE_AppDefined, "  [Failure]\t%s", ex.what()); \
        return result;                                                    \
    }                                                                     \
    catch (...)                                                           \
    {                                                                     \
        SINFO(CE_Fatal, CPLE_AppDefined, "  [Fatal]\tUnknown exception"); \
        return result;                                                    \
    }
#define SL(func, ...) \
    makeTimeLogFunction(func, #func, __FILE__, #__VA_ARGS__, __LINE__)(__VA_ARGS__)
#define SLC6(obj, func, ...) \
    makeTimeLogClassFunction(&func, &obj, #func, __FILE__, #__VA_ARGS__, __LINE__)(__VA_ARGS__)
#else
#define SSF
#define SEF(result)
#define SL(func, ...) func(__VA_ARGS__)
#define SLC6(obj, func, ...) std::bind(&func, &obj, __VA_ARGS__)()
#endif // _SLOG_DEBUG

#endif // _SLOG_H_
