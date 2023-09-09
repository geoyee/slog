#ifndef _SLOG_H_
#define _SLOG_H_

#include <iostream>
#include <string>
#include <functional>
#include <ctime>
#include <chrono>
#include <cassert>
#include <type_traits>

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
#define SINFO(eErrClass, err_no, ARGS...) \
    {                                     \
        fprintf(stderr, ARGS);            \
        fprintf(stderr, "\n");            \
        if (eErrClass == CE_Fatal)        \
            exit(1);                      \
    }                                     \
    while (0)
#endif

std::string getNowTimeStr()
{
    time_t t = std::time(nullptr);
    char buffer[128];
    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y/%m/%d %H:%M:%S",
        std::localtime(&t));
    return std::string(buffer);
}

template <typename>
class TimeLog;

template <typename RET, typename... ARGS>
class TimeLog<RET(ARGS...)>
{
private:
    std::function<RET(ARGS...)> _func;

public:
    TimeLog(std::function<RET(ARGS...)> func) : _func(func) {}
    RET operator()(ARGS... args)
    {
        SINFO(CE_Debug, CPLE_None, "[%s] %s", getNowTimeStr().c_str(), __FUNCTION__);
        auto start_time = std::chrono::high_resolution_clock::now();
        try
        {
            RET result = _func(args...);
            auto duration = std::chrono::high_resolution_clock::now() - start_time;
            float ms = duration.count() * 1000.0f;
            SINFO(CE_Debug, CPLE_None, "[Finished] %f ms", ms);
            return result;
        }
        catch (const std::exception &ex)
        {
            SINFO(CE_Failure,
                  CPLE_AppDefined,
                  "[Failure] %s (%d): %s", __FILE__, __LINE__, ex.what());
            return RET();
        }
        catch (...)
        {
            SINFO(CE_Fatal,
                  CPLE_AppDefined,
                  "[Fatal] %s (%d): Unknown exception", __FILE__, __LINE__);
            return RET();
        }
    }
};

template <typename RET, typename... ARGS>
TimeLog<RET(ARGS...)> makeTimeLogFunction(RET (*func)(ARGS...))
{
    return TimeLog<RET(ARGS...)>(std::function<RET(ARGS...)>(func));
}

#ifdef _DEBUG
#define SErr(func) makeTimeLogFunction(func)
#else
#define SErr(func) func
#endif // _DEBUG

#endif // _SLOG_H_