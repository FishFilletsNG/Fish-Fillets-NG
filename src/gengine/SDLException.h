#ifndef HEADER_SDLEXCEPTION_H
#define HEADER_SDLEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about SDL fail.
 */
class SDLException : public BaseException  {
    public:
        SDLException(const ExInfo &problem);
};

#endif
