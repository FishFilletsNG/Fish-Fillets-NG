#ifndef HEADER_MOUSESTROKE_H
#define HEADER_MOUSESTROKE_H

#include "V2.h"

#include "SDL.h"
#include <string>

/**
 * Contains info about mouse click.
 */
class MouseStroke {
    private:
        Uint8 m_button;
        V2 m_loc;
    public:
        MouseStroke(const SDL_MouseButtonEvent &event);

        bool isLeft() const { return m_button == SDL_BUTTON_LEFT; }
        bool isMiddle() const { return m_button == SDL_BUTTON_MIDDLE; }
        bool isRight() const { return m_button == SDL_BUTTON_RIGHT; }
        V2 getLoc() const { return m_loc; }
        std::string toString() const;
};

#endif
