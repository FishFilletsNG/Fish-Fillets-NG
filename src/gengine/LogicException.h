#ifndef HEADER_LOGICEXCEPTION_H
#define HEADER_LOGICEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about program fail.
 */
class LogicException : public BaseException  {
    public:
        LogicException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
