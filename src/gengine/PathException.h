#ifndef HEADER_PATHEXCEPTION_H
#define HEADER_PATHEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about filename and filesystem.
 */
class PathException : public BaseException  {
    public:
        PathException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
