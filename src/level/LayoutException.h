#ifndef HEADER_LAYOUTEXCEPTION_H
#define HEADER_LAYOUTEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about placement fail.
 */
class LayoutException : public BaseException  {
    public:
        LayoutException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
