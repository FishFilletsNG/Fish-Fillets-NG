#ifndef HEADER_KEYCONTROL_H
#define HEADER_KEYCONTROL_H

#include "SDL.h"

/**
 * Keyboard controls.
 */
class KeyControl {
    private:
        SDL_Keycode m_up;
        SDL_Keycode m_down;
        SDL_Keycode m_left;
        SDL_Keycode m_right;
    public:
        KeyControl();
        void setUp(SDL_Keycode key) { m_up = key; }
        void setDown(SDL_Keycode key) { m_down = key; }
        void setLeft(SDL_Keycode key) { m_left = key; }
        void setRight(SDL_Keycode key) { m_right = key; }

        SDL_Keycode getUp() const { return m_up; }
        SDL_Keycode getDown() const { return m_down; }
        SDL_Keycode getLeft() const { return m_left; }
        SDL_Keycode getRight() const { return m_right; }
};

#endif
