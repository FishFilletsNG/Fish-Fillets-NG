#ifndef HEADER_EXINFO_H
#define HEADER_EXINFO_H

#include "ExInfo.h"

#include <string>

/**
 * Detailed info.
 */
class ExInfo {
    private:
        std::string m_what;
    public:
        explicit ExInfo(const std::string &problem) throw();
        const char *what() const throw();
        std::string info() const throw() { return m_what; }

        ExInfo &addInfo(const std::string &name,
                const std::string &value) throw();
        ExInfo &addInfo(const std::string &name, long value) throw();
};

#endif
