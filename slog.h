/*
 @ brief:   A simple log system for C++14
 @ author:  Yizhou Chen
 @ date:    2023-09-17
 @ version: 1.2.0
 */

#ifndef _SLOG_H_
#define _SLOG_H_

#pragma warning(disable : 4996) // Disable: 'localtime': This function or variable may be unsafe

#include <iostream>
#include <string>
#include <functional>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cassert>
#include <type_traits>
#include <utility>

#ifdef CPL_ERROR_H_INCLUDED // Use CPLError

#include <cpl_error.h>
#define SINFO CPLError

#else // Use custom error

#include <mutex>

static std::mutex oAllMutex;

#define SLOCK oAllMutex.lock()
#define SUNLOCK oAllMutex.unlock()

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

#define SINFO(eErrClass, err_no, ...) \
    {                                 \
        SLOCK;                        \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
        if (eErrClass == CE_Fatal)    \
            exit(1);                  \
        SUNLOCK;                      \
    }                                 \
    while (0)

#endif // CPL_ERROR_H_INCLUDED

// Get current time
std::string nowTimeStr()
{
    std::time_t t = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer,
                  sizeof(buffer),
                  "%Y/%m/%d %H:%M:%S",
                  std::localtime(&t));
    return std::string(buffer);
}

/*
 @ author:   Garcia6l20
 @ refrence: https://github.com/Garcia6l20/if_constexpr14
 */
namespace ic
{
    namespace detail
    {
        template <bool result, typename TrueT, typename FalseT = std::nullptr_t>
        struct if_constexpr
        {
            TrueT true_;
            FalseT false_;

            constexpr explicit if_constexpr(TrueT trueT, FalseT falseT = nullptr)
                : true_{std::move(trueT)}, false_{std::move(falseT)} {}

            template <bool check = result, std::enable_if_t<check, int> = 0>
            constexpr auto operator()()
            {
                return true_();
            }

            template <bool check = result,
                      std::enable_if_t<!check && !std::is_same<FalseT, std::nullptr_t>::value, int> = 0>
            constexpr auto operator()()
            {
                return false_();
            }

            template <bool check = result,
                      std::enable_if_t<!check && std::is_same<FalseT, std::nullptr_t>::value, int> = 0>
            constexpr void operator()() {}
        };

        template <typename ThenT>
        struct else_
        {
            ThenT then_;
            constexpr explicit else_(ThenT then)
                : then_{std::move(then)} {}
        };

        template <class T, template <class...> class Template>
        struct is_specialization : std::false_type
        {
        };

        template <template <class...> class Template, class... Args>
        struct is_specialization<Template<Args...>, Template> : std::true_type
        {
        };

        template <bool result, typename CaseT>
        struct case_constexpr
        {
            static constexpr bool value = result;
            CaseT case_;
            constexpr explicit case_constexpr(CaseT &&case_)
                : case_{std::move(case_)} {}
            constexpr auto operator()()
            {
                return case_();
            }
        };
    }

    template <bool result, typename TrueT, typename ElseT,
              std::enable_if_t<detail::is_specialization<ElseT, detail::else_>::value, int> = 0>
    constexpr auto if_(TrueT &&trueT, ElseT &&else_)
    {
        return detail::if_constexpr<result, TrueT, decltype(else_.then_)>(
            std::forward<TrueT>(trueT), std::move(else_.then_))();
    }

    template <bool result, typename TrueT, typename ElseT,
              std::enable_if_t<!detail::is_specialization<ElseT, detail::else_>::value, int> = 0>
    constexpr auto if_(TrueT &&trueT, ElseT &&else_)
    {
        auto fwd = [else_ = std::forward<decltype(else_)>(else_)]() mutable
        {
            return else_();
        };
        return detail::if_constexpr<result, TrueT, decltype(fwd)>(
            std::forward<TrueT>(trueT), std::move(fwd))();
    }

    template <bool result, typename TrueT, typename ElseT>
    constexpr auto else_if_(TrueT &&trueT, ElseT &&else_)
    {
        return detail::if_constexpr<result, TrueT, decltype(else_.then_)>(
            std::forward<TrueT>(trueT), std::move(else_.then_));
    }

    template <bool result, typename TrueT>
    constexpr auto else_if_(TrueT &&trueT)
    {
        auto nop = [] {};
        return detail::if_constexpr<result, TrueT, decltype(nop)>(
            std::forward<TrueT>(trueT), std::move(nop));
    }

    template <typename ThenT>
    constexpr auto else_(ThenT &&then)
    {
        return detail::else_<ThenT>(std::forward<ThenT>(then));
    }

    template <bool result, typename CaseT>
    constexpr auto case_(CaseT &&case_)
    {
        return detail::case_constexpr<result, CaseT>{std::forward<CaseT>(case_)};
    }

    template <typename DefaultT>
    constexpr auto default_(DefaultT &&default_)
    {
        return detail::case_constexpr<true, DefaultT>{std::forward<DefaultT>(default_)};
    }

    template <typename LastT,
              std::enable_if_t<LastT::value, int> = 0>
    constexpr auto switch_(LastT &&last)
    {
        return last();
    }

    template <typename LastT,
              std::enable_if_t<!LastT::value, int> = 0>
    constexpr auto switch_(LastT &&last)
    {
    }

    template <typename FirstT, typename... CasesT,
              std::enable_if_t<FirstT::value, int> = 0>
    constexpr auto switch_(FirstT &&first, CasesT &&...cases)
    {
        return first();
    }

    template <typename FirstT, typename... CasesT,
              std::enable_if_t<!FirstT::value, int> = 0>
    constexpr auto switch_(FirstT &&first, CasesT &&...cases)
    {
        return switch_<CasesT...>(std::forward<CasesT>(cases)...);
    }
}

// Return NaN of the different types
// Add and Update as needed
template <typename T>
constexpr T NaN()
{
    return ic::switch_(
        ic::case_<std::is_pointer<T>::value>(
            []
            { return nullptr; }),
        ic::case_<std::is_same<T, CPLErr>::value>(
            []
            { return CE_Failure; }),
        ic::case_<std::is_same<T, bool>::value>(
            []
            { return false; }),
        ic::case_<std::is_same<T, char>::value>(
            []
            { return std::numeric_limits<char>::min(); }),
        ic::case_<std::is_same<T, signed char>::value>(
            []
            { return std::numeric_limits<signed char>::min(); }),
        ic::case_<std::is_same<T, unsigned char>::value>(
            []
            { return std::numeric_limits<unsigned char>::min(); }),
        ic::case_<std::is_same<T, wchar_t>::value>(
            []
            { return std::numeric_limits<wchar_t>::min(); }),
        ic::case_<std::is_same<T, char16_t>::value>(
            []
            { return std::numeric_limits<char16_t>::min(); }),
        ic::case_<std::is_same<T, char32_t>::value>(
            []
            { return std::numeric_limits<char32_t>::min(); }),
        ic::case_<std::is_same<T, short>::value>(
            []
            { return std::numeric_limits<short>::min(); }),
        ic::case_<std::is_same<T, unsigned short>::value>(
            []
            { return std::numeric_limits<unsigned short>::min(); }),
        ic::case_<std::is_same<T, int>::value>(
            []
            { return std::numeric_limits<int>::min(); }),
        ic::case_<std::is_same<T, unsigned int>::value>(
            []
            { return std::numeric_limits<unsigned int>::min(); }),
        ic::case_<std::is_same<T, long>::value>(
            []
            { return std::numeric_limits<long>::min(); }),
        ic::case_<std::is_same<T, unsigned long>::value>(
            []
            { return std::numeric_limits<unsigned long>::min(); }),
        ic::case_<std::is_same<T, long long>::value>(
            []
            { return std::numeric_limits<long long>::min(); }),
        ic::case_<std::is_same<T, unsigned long long>::value>(
            []
            { return std::numeric_limits<unsigned long long>::min(); }),
        ic::case_<std::is_same<T, float>::value>(
            []
            { return std::numeric_limits<float>::quiet_NaN(); }),
        ic::case_<std::is_same<T, double>::value>(
            []
            { return std::numeric_limits<double>::quiet_NaN(); }),
        ic::case_<std::is_same<T, long double>::value>(
            []
            { return std::numeric_limits<long double>::quiet_NaN(); }),
        ic::default_(
            []
            { return T(); }));
}

// Select placeholders according to the number of arguments
// In c++17, it can be replaced by if constexpr
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

// Select function according to the return type
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
    return void();
}

// core class of slog
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
            return NaN<RET>();
        }
        catch (...)
        {
            SINFO(CE_Fatal, CPLE_AppDefined, "  [Fatal]\tUnknown exception");
            return NaN<RET>();
        }
    }
};

// Make slog function
template <typename RET, typename... ARGS>
constexpr TimeLog<RET(ARGS...)> makeTimeLogFunction(RET (*func)(ARGS...),
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
constexpr TimeLog<RET(ARGS...)> makeTimeLogMemberFunction(RET (CLS::*func)(ARGS...),
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

template <typename RET, typename... ARGS>
constexpr auto decorateFunction(RET (*func)(ARGS...),
                                const char *func_name,
                                const char *file_name,
                                int line_no)
{
    return [=](ARGS... args) -> RET
    {
        return makeTimeLogFunction(func, func_name, file_name, "...", line_no)(args...);
    };
}

template <typename RET, typename CLS, typename... ARGS>
constexpr auto decorateMemberFunction(RET (CLS::*func)(ARGS...),
                                      CLS *obj,
                                      const char *func_name,
                                      const char *file_name,
                                      int line_no)
{
    return [=](ARGS... args) -> RET
    {
        return makeTimeLogMemberFunction(func, obj, func_name, file_name, "...", line_no)(args...);
    };
}

#define VALIDATE_ARGUMENT0(arg, func)                                                \
    do                                                                               \
    {                                                                                \
        if (arg == NaN<decltype(arg)>())                                             \
        {                                                                            \
            SINFO(CE_Debug, CPLE_None, "- %s", nowTimeStr().c_str());                \
            SINFO(CE_Debug, CPLE_None, "  [Function]\t%s", func);                    \
            SINFO(CE_Debug, CPLE_None, "  [Location]\t%s (%d)", __FILE__, __LINE__); \
            SINFO(CE_Failure, CPLE_NotSupported,                                     \
                  "  [Failure]\tArgument \'%s\' is NaN", #arg);                      \
            return;                                                                  \
        }                                                                            \
    } while (0)

#define VALIDATE_ARGUMENT1(arg, func, ret)                                           \
    do                                                                               \
    {                                                                                \
        if (arg == NaN<decltype(arg)>())                                             \
        {                                                                            \
            SINFO(CE_Debug, CPLE_None, "- %s", nowTimeStr().c_str());                \
            SINFO(CE_Debug, CPLE_None, "  [Function]\t%s", func);                    \
            SINFO(CE_Debug, CPLE_None, "  [Location]\t%s (%d)", __FILE__, __LINE__); \
            SINFO(CE_Failure, CPLE_NotSupported,                                     \
                  "  [Failure]\tArgument \'%s\' is NaN", #arg);                      \
            return ret;                                                              \
        }                                                                            \
    } while (0)

#ifdef _ENABLE_SLOG

#define SENTRY                                                               \
    SINFO(CE_Debug, CPLE_None, "- %s", nowTimeStr().c_str());                \
    SINFO(CE_Debug, CPLE_None, "  [Function]\t%s", __FUNCTION__);            \
    SINFO(CE_Debug, CPLE_None, "  [Location]\t%s (%d)", __FILE__, __LINE__); \
    try                                                                      \
    {

#define SLEAVE(ret)                                                       \
    }                                                                     \
    catch (const std::exception &ex)                                      \
    {                                                                     \
        SINFO(CE_Failure, CPLE_AppDefined, "  [Failure]\t%s", ex.what()); \
        return ret;                                                       \
    }                                                                     \
    catch (...)                                                           \
    {                                                                     \
        SINFO(CE_Fatal, CPLE_AppDefined, "  [Fatal]\tUnknown exception"); \
        return ret;                                                       \
    }

#define SFUNC_DEC(func) decorateFunction(&func, #func, __FILE__, __LINE__)

#define SFUNC_MEM_DEC(obj, func) decorateMemberFunction(&func, &obj, #func, __FILE__, __LINE__)

#define SFUNC_RUN(func, ...) \
    makeTimeLogFunction(func, #func, __FILE__, #__VA_ARGS__, __LINE__)(__VA_ARGS__)

#define SFUNC_MEM_RUN(obj, func, ...) \
    makeTimeLogMemberFunction(&func, &obj, #func, __FILE__, #__VA_ARGS__, __LINE__)(__VA_ARGS__)

#else

#define SENTRY
#define SLEAVE(result)
#define SFUNC_DEC(func) func
#define SFUNC_BIND_DEC(func) func
#define SFUNC_RUN(func, ...) func(__VA_ARGS__)
#define SFUNC_MEM_RUN(obj, func, ...) std::bind(&func, &obj, __VA_ARGS__)()

#endif // _ENABLE_SLOG

#endif // _SLOG_H_