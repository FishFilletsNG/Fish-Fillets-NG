#ifndef HEADER_VIEWEFFECT_H
#define HEADER_VIEWEFFECT_H

#include "SDL.h"

/**
 * Graphic effect before blit.
 */
class ViewEffect {
    public:
        enum eEffect {
            EFFECT_NONE,
            EFFECT_DISINTEGRATE,
            EFFECT_MIRROR,
            EFFECT_INVISIBLE,
            EFFECT_REVERSE
        };
    private:
        static const int DISINT_START = 400;
        static const int DISINT_SPEED = 30;
        static const int MIRROR_BORDER = 3;
        eEffect m_effect;
        int m_disint;
    private:
        void blitNone(SDL_Surface *screen, SDL_Surface *surface, int x, int y);
        void blitDisInt(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
        void blitMirror(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
        void blitReverse(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);

    public:
        ViewEffect();
        void setEffect(eEffect effect);
        void updateEffect();
        bool isDisintegrated() const;
        bool isInvisible() const;
        void blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y);
};

#endif
