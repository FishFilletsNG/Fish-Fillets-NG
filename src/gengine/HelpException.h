#ifndef HEADER_HELPEXCEPTION_H
#define HEADER_HELPEXCEPTION_H

#include "BaseException.h"

/**
 * Only help is need.
 */
class HelpException : public BaseException  {
    public:
        HelpException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
