#ifndef HEADER_LOADEXCEPTION_H
#define HEADER_LOADEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about load game fail.
 */
class LoadException : public BaseException  {
    public:
        LoadException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
