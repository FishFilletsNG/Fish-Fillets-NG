#ifndef HEADER_NAMEEXCEPTION_H
#define HEADER_NAMEEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about name fail.
 */
class NameException : public BaseException  {
    public:
        NameException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
