#ifndef HEADER_KEYSTROKE_H
#define HEADER_KEYSTROKE_H

#include "SDL.h"
#include <string>

/**
 * KeyStroke.
 */
class KeyStroke {
    private:
        static const int STROKE_IGNORE = ~(KMOD_CTRL|KMOD_ALT);
        SDLKey m_sym;
        int m_mod;
        Uint16 m_unicode;
    private:
        static int modStrip(int mod);
    public:
        KeyStroke(const SDL_keysym &keysym);
        KeyStroke(SDLKey sym, int mod);

        SDLKey getKey() const { return m_sym; }
        Uint16 getUnicode() const { return m_unicode; }
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
