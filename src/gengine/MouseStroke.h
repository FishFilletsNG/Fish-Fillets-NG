#ifndef HEADER_MOUSESTROKE_H
#define HEADER_MOUSESTROKE_H

#include "V2.h"

#include "SDL.h"
#include <string>

/**
 * Contains info mouse button press.
 */
class MouseStroke {
    private:
        Uint8 m_button;
        V2 m_loc;
    public:
        MouseStroke(const SDL_MouseButtonEvent &event);
        MouseStroke(Uint8 button, int x=0, int y=0);

        V2 getLoc() const { return m_loc; }
        bool equals(const MouseStroke &other) const;
        std::string toString() const;
};

#endif
