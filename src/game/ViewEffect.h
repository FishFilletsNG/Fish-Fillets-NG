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
        void blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y);

        void setEffect(eEffect effect) { m_effect = effect; }
        void setDisInt(int disint) { m_disint = disint; }
};

#endif
