#pragma once
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>

#undef TRACE
#undef DEBUG
#undef INFO
#undef WARN
#undef ERROR
#undef FATAL

#define LOGGER_INSTANCE phi::Singleton<phi::Logger>::instance()

#define DEBUG DEBUG
#ifdef DEBUG
#define LOG(level, message) LOGGER_INSTANCE->log(phi::Logger::Level::level, message, __LINE__, __FILE__)
#define LOG_F(level, message, ...) LOGGER_INSTANCE->log(phi::Logger::Level::level, message, __LINE__, __FILE__, __VA_ARGS__)
#else
#define LOG(level, message)
#define LOG_F(level, message, ...)
#endif

#define LTRACE(message) LOG(TRACE, message)
#define LDEBUG(message) LOG(DEBUG, message)
#define LINFO(message) LOG(INFO, message)
#define LWARN(message) LOG(WARN, message)
#define LERROR(message) LOG(ERROR, message)
#define LFATAL(message) LOG(FATAL, message)
#define LTRACE_F(message, ...) LOG_F(TRACE, message, __VA_ARGS__)
#define LDEBUG_F(message, ...) LOG_F(DEBUG, message, __VA_ARGS__)
#define LINFO_F(message, ...) LOG_F(INFO, message, __VA_ARGS__)
#define LWARN_F(message, ...) LOG_F(WARN, message, __VA_ARGS__)
#define LERROR_F(message, ...) LOG_F(ERROR, message, __VA_ARGS__)
#define LFATAL_F(message, ...) LOG_F(FATAL, message, __VA_ARGS__)

namespace phi
{
    class Logger
    {
    public:
        enum Level
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            MAX
        };

    private:
        std::ofstream m_file;
        Level m_level;

    public:
        Logger();
        Logger(const Level &level);
        Logger(const char *filename, const Level &level);
        ~Logger();

        Logger(const Logger &) = delete;
        Logger(Logger &&) = delete;
        Logger &operator=(const Logger &) = delete;
        Logger &operator=(Logger &&) = delete;

        void setLevel(const Level &);
        static std::string getLevelString(const Level &);

        void log(const Level &, const std::string &, uint64_t, const char *);
        void log(const Level &, const char *, uint64_t, const char *);
        template <typename... Args>
        void log(const Level &, const char *, uint64_t, const char *, Args... args);
    };

    template <typename... Args>
    inline void Logger::log(const Level &level, const char *fmt, uint64_t line, const char *filename, Args... args)
    {
        char *buffer = new char[0xffff];
        sprintf(buffer, fmt, args...);
        log(level, buffer, line, filename);
        delete[] buffer;
    }
} // namespace phi
