#ifndef HEADER_IKEYCONSOLE_H
#define HEADER_IKEYCONSOLE_H

#include "Drawable.h"

#include "SDL.h"

/**
 * Console interface.
 */
class IKeyConsole : public Drawable {
    public:
        virtual ~IKeyConsole() {}

        virtual void keyDown(const SDL_keysym &keysym) = 0;
};

#endif
