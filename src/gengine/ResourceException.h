#ifndef HEADER_RESOURCEEXCEPTION_H
#define HEADER_RESOURCEEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about not found resource.
 */
class ResourceException : public BaseException  {
    public:
        ResourceException(const ExInfo &problem)
            : BaseException(problem)
            {}
};

#endif
