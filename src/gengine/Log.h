#ifndef HEADER_LOG_H
#define HEADER_LOG_H

#include "ExInfo.h"

/**
 * Log utils.
 */
class Log {
    private:
        static int ms_logLevel;
    public:
        /**
         * Log priority, like syslog.
         */
        static const int LEVEL_DEBUG = 7;
        static const int LEVEL_INFO = 6;
        static const int LEVEL_WARNING = 4;
        static const int LEVEL_ERROR = 3;

    public:
        static void setLogLevel(int logLevel) { ms_logLevel = logLevel; }
        static int getLogLevel() { return ms_logLevel; }

        static void log(int level, const char *file, int line,
                const ExInfo &info) throw();
};

#define LOG_DEBUG(info) do { Log::log(Log::LEVEL_DEBUG, __FILE__, __LINE__, info); } while (false)
#define LOG_INFO(info) do { Log::log(Log::LEVEL_INFO, __FILE__, __LINE__, info); } while (false)
#define LOG_WARNING(info) do { Log::log(Log::LEVEL_WARNING, __FILE__, __LINE__, info); } while (false)
#define LOG_ERROR(info) do { Log::log(Log::LEVEL_ERROR, __FILE__, __LINE__, info); } while (false)

#endif
