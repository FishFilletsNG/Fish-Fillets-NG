#ifndef HEADER_IKEYCONSOLE_H
#define HEADER_IKEYCONSOLE_H

#include "IDrawer.h"

#include "SDL.h"

/**
 * Console interface.
 */
class IKeyConsole : public IDrawer {
    public:
        virtual ~IKeyConsole() {}

        virtual void keyDown(const SDL_keysym &keysym) = 0;
};

#endif
