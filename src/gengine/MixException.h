#ifndef HEADER_MIXEXCEPTION_H
#define HEADER_MIXEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about SDL_mixer fail.
 */
class MixException : public BaseException  {
    public:
        MixException(const ExInfo &problem);
};

#endif
