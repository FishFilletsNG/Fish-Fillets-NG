#ifndef HEADER_BASEEXCEPTION_H
#define HEADER_BASEEXCEPTION_H

#include "ExInfo.h"

#include <stdexcept>

/**
 * Generic exception.
 */
class BaseException : public std::exception {
    private:
        ExInfo m_info;
    public:
        explicit BaseException(const ExInfo &exinfo);
        virtual ~BaseException() throw() {}
        virtual const char *what() const throw();
        virtual const ExInfo &info() const throw();
};

#endif
