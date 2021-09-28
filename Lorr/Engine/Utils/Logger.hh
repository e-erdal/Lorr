#pragma once

#include <memory>

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define CHECK(expr, fail_msg, ...)                                                                                                                                       \
    if (!(expr)) LOG_ERROR(fail_msg, ##__VA_ARGS__);

#define ENABLE_LOG_TRACE
#ifdef ENABLE_LOG_TRACE
    #define LOG_TRACE(...) Console::GetCoreLogger()->trace(__VA_ARGS__)
#else
    #define LOG_TRACE(...) (void *)0
#endif

#define LOG_INFO(...) Console::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Console::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)                                                                                                                                                   \
    {                                                                                                                                                                    \
        Console::GetCoreLogger()->error(__VA_ARGS__);                                                                                                                    \
        Console::GetCoreLogger()->dump_backtrace();                                                                                                                      \
        Console::GetCoreLogger()->flush();                                                                                                                               \
        abort();                                                                                                                                                         \
    }

namespace Lorr
{
    class Console
    {
    private:
        inline static std::shared_ptr<spdlog::logger> s_pCoreLogger = nullptr;
        inline static bool g_LoggerInitialized = false;

    public:
        static void Init()
        {
            if (g_LoggerInitialized) return;

            std::vector<spdlog::sink_ptr> logSinks;

#if PLATFORM_ANDROID
            // ANDROID
            logSinks.emplace_back(std::make_shared<spdlog::sinks::android_sink_mt>());
            logSinks[0]->set_pattern("%T %5^%l%$\t| %v");
#else
            // PC
            logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("lorr.log", true));

            logSinks[0]->set_pattern("%Y-%m-%d_%T.%e | %5^%L%$ | %v");
            logSinks[1]->set_pattern("%Y-%m-%d_%T.%e | %L | %v");
#endif

            s_pCoreLogger = std::make_shared<spdlog::logger>("Engine", begin(logSinks), end(logSinks));
            spdlog::register_logger(s_pCoreLogger);
            s_pCoreLogger->set_level(spdlog::level::trace);
            s_pCoreLogger->flush_on(spdlog::level::trace);

            g_LoggerInitialized = true;
        }

        static std::shared_ptr<spdlog::logger> &GetCoreLogger()
        {
            return s_pCoreLogger;
        }
    };

}  // namespace Lorr