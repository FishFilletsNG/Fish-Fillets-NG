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
            EFFECT_MIRROR
        };
    private:
        static const int DISINT_START = 400;
        static const int DISINT_SPEED = 30;
        eEffect m_effect;
        int m_disint;
    private:
        void blitNone(SDL_Surface *screen, SDL_Surface *surface, int x, int y);
        void blitDisInt(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
        Uint8 getAlpha(Uint32 pixel, SDL_PixelFormat *fmt);
        Uint32
            ViewEffect::getPixel(SDL_Surface *surface, int x, int y);
        void
            ViewEffect::putPixel(SDL_Surface *surface, int x, int y,
                    Uint32 pixel);
    public:
        ViewEffect();
        void setEffect(eEffect effect);
        void updateEffect();
        bool isInvisible() const;
        void blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y);
};

#endif
