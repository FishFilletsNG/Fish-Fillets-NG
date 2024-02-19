#ifndef HEADER_KEYCONTROL_H
#define HEADER_KEYCONTROL_H

#include "SDL.h"

/**
 * Keyboard controls.
 */
class KeyControl {
    private:
        SDL_Scancode m_up;
        SDL_Scancode m_down;
        SDL_Scancode m_left;
        SDL_Scancode m_right;
    public:
        KeyControl();
        void setUp(SDL_Scancode key) { m_up = key; }
        void setDown(SDL_Scancode key) { m_down = key; }
        void setLeft(SDL_Scancode key) { m_left = key; }
        void setRight(SDL_Scancode key) { m_right = key; }

        SDL_Scancode getUp() const { return m_up; }
        SDL_Scancode getDown() const { return m_down; }
        SDL_Scancode getLeft() const { return m_left; }
        SDL_Scancode getRight() const { return m_right; }
};

#endif
