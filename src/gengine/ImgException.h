#ifndef HEADER_IMGEXCEPTION_H
#define HEADER_IMGEXCEPTION_H

#include "BaseException.h"

/**
 * Exception about SDL_image fail.
 */
class ImgException : public BaseException  {
    public:
        ImgException(const ExInfo &problem);
};

#endif
