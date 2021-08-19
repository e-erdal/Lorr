#pragma once

#include <memory>

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define CHECK(expr, fail_msg, ...)                                                                                                                                       \
    if (!(expr)) Console::Fatal(fail_msg, ##__VA_ARGS__);

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

            logSinks[0]->set_pattern("%Y-%m-%d_%T.%e | %5^%l%$\t  | %v");
            logSinks[1]->set_pattern("%Y-%m-%d_%T.%e | %l\t  | %v");
#endif

            s_pCoreLogger = std::make_shared<spdlog::logger>("Engine", begin(logSinks), end(logSinks));
            spdlog::register_logger(s_pCoreLogger);
            s_pCoreLogger->set_level(spdlog::level::trace);
            s_pCoreLogger->flush_on(spdlog::level::trace);

            g_LoggerInitialized = true;
        }

        template<typename T>
        static void Log(const T &msg)
        {
            s_pCoreLogger->debug(msg);
        }

        template<typename T>
        static void Trace(const T &msg)
        {
            s_pCoreLogger->trace(msg);
        }

        template<typename T>
        static void Warn(const T &msg)
        {
            s_pCoreLogger->warn(msg);
        }

        template<typename T>
        static void Error(const T &msg)
        {
            s_pCoreLogger->error(msg);
        }

        template<typename T>
        static void Fatal(const T &msg)
        {
            s_pCoreLogger->critical(msg);
            s_pCoreLogger->dump_backtrace();
            s_pCoreLogger->flush();
            abort();
        }

        template<typename FormatString, typename... Args>
        static void Log(const FormatString &fmt, Args &&...args)
        {
            s_pCoreLogger->debug(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        static void Trace(const FormatString &fmt, Args &&...args)
        {
            s_pCoreLogger->trace(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        static void Info(const FormatString &fmt, Args &&...args)
        {
            s_pCoreLogger->info(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        static void Warn(const FormatString &fmt, Args &&...args)
        {
            s_pCoreLogger->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        static void Error(const FormatString &fmt, Args &&...args)
        {
            s_pCoreLogger->error(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        static void Fatal(const FormatString &fmt, Args &&...args)
        {
            s_pCoreLogger->error(fmt, std::forward<Args>(args)...);
            s_pCoreLogger->flush();
            abort();
        }

        static std::shared_ptr<spdlog::logger> &GetCoreLogger()
        {
            return s_pCoreLogger;
        }
    };

}  // namespace Lorr