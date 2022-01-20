#pragma once

#include <loguru.hpp>

#define CHECK(expr, fail_msg, ...)                                                                                                                   \
    if (!(expr)) LOG_ERROR(fail_msg, ##__VA_ARGS__);

#define ENABLE_LOG_TRACE
#ifdef ENABLE_LOG_TRACE
#define LOG_TRACE(...) DLOG_F(INFO, __VA_ARGS__)
#else
#define LOG_TRACE(...) (void *)0
#endif

#define LOG_INFO(...) LOG_F(INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG_F(WARNING, __VA_ARGS__)
#define LOG_ERROR(...)                                                                                                                               \
    {                                                                                                                                                \
        ABORT_F(__VA_ARGS__);                                                                                                                        \
        abort();                                                                                                                                     \
    }

namespace lr
{
    class Console
    {
    public:
        static void Init()
        {
#if _DEBUG
            loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
            loguru::add_file("lorr.log", loguru::Truncate, loguru::Verbosity_MAX);
#else
            loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
            loguru::add_file("lorr.log", loguru::Truncate, loguru::Verbosity_INFO);
#endif
        }
    };

}  // namespace lr