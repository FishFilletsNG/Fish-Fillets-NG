#ifndef HEADER_TTFEXCEPTION_H
#define HEADER_TTFEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about SDL_ttf fail.
 */
class TTFException : public BaseException  {
    public:
        TTFException(const ExInfo &problem);
};

#endif
