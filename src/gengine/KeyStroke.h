#ifndef HEADER_KEYSTROKE_H
#define HEADER_KEYSTROKE_H

#include "SDL.h"
#include <string>

/**
 * KeyStroke.
 */
class KeyStroke {
    private:
        static const int STROKE_IGNORE = ~(KMOD_SHIFT|KMOD_CTRL|KMOD_ALT);
        SDLKey m_sym;
        int m_mod;
    private:
        static int modStrip(int mod);
    public:
        KeyStroke(const SDL_keysym &keysym);
        KeyStroke(SDLKey sym, int mod);

        SDLKey getKey() const { return m_sym; }
        bool less(const KeyStroke &other) const;
        bool equals(const KeyStroke &other) const;
        std::string toString() const;
};

/**
 * KeyStroke comparation.
 */
struct stroke_less
{
    bool operator()(const KeyStroke &left, const KeyStroke &right) const
    {
        return left.less(right);
    }
};

#endif
