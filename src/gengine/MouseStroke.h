#ifndef HEADER_MOUSESTROKE_H
#define HEADER_MOUSESTROKE_H

#include "SDL.h"
#include <string>

/**
 * Contains info mouse button press.
 */
class MouseStroke {
    private:
        Uint8 m_button;
        int m_x;
        int m_y;
    public:
        MouseStroke(const SDL_MouseButtonEvent &event);
        MouseStroke(Uint8 button, int x=0, int y=0);

        int getX() const { return m_x; }
        int getY() const { return m_y; }
        bool equals(const MouseStroke &other) const;
        std::string toString() const;
};

#endif
