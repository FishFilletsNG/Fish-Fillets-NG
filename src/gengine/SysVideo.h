#ifndef HEADER_SYSVIDEO_H
#define HEADER_SYSVIDEO_H

#include <string>
#include <SDL.h>

/**
 * System dependend video functions.
 */
class SysVideo {
    public:
        static void setCaption(SDL_Window *window, const std::string &title);
};

#endif
