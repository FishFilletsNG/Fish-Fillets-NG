#ifndef HEADER_SCRIPTEXCEPTION_H
#define HEADER_SCRIPTEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about script fail.
 */
class ScriptException : public BaseException  {
    public:
        ScriptException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
