#include "logger.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <cstring>

namespace phi
{
    static std::string getTimeString(const char *format = "%Y%m%d-%H:%M:%S")
    {
        using namespace std;
        auto now = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now());
        time_t t = chrono::system_clock::to_time_t(now);
        int milliSecond = now.time_since_epoch().count() % 1000;
        auto localTm = gmtime(&t);
        char buff[32];
        int len = strftime(buff, 32, format, localTm);
        sprintf(buff + len, ".%03u", milliSecond);
        return string(buff);
    }

    static inline std::string getFilename()
    {
        return "./logs/" + getTimeString("%Y%m%d-%H_%M_%S") + ".log";
    }

    Logger::Logger() : m_file(getFilename()), m_level(Level::INFO)
    {
    }

    Logger::Logger(const Level &level) : m_file(getFilename()), m_level(level)
    {
    }

    Logger::Logger(const char *filename, const Level &level) : m_level(level), m_file(filename)
    {
    }

    Logger::~Logger()
    {
        m_file.flush();
        m_file.close();
    }

    void Logger::setLevel(const Level &level)
    {
        m_level = level;
    }

    std::string Logger::getLevelString(const Level &level)
    {
        switch (level)
        {
        case Level::TRACE:
            return "TRACE";
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARN:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::FATAL:
            return "FATAL";
        default:
            return std::string();
        }
    }

    void Logger::log(const Level &level, const std::string &message, uint64_t line, const char *filename)
    {
        log(level, message.c_str(), line, filename);
    }

    void Logger::log(const Level &level, const char *message, uint64_t line, const char *filename)
    {
        if (level < m_level)
        {
            return;
        }

        std::ostringstream ss;
        ss << getLevelString(level) << "\t[";
        ss << getTimeString() << "]\t";

        if (*filename != '.') ss << "./";
        size_t length = std::strlen(filename);
        for (size_t i = 0; i < length; i++)
        {
            ss << (filename[i] == '\\' ? '/' : filename[i]);
        }
        
        ss << ":" << line << "\t\t";
        ss << message << std::endl;

        m_file << ss.str();
        #ifdef DETAIL
        std::cout << ss.str();
        #endif
    }
} // namespace phi