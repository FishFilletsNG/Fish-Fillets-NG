#ifndef HEADER_EFFECTINVISIBLE_H
#define HEADER_EFFECTINVISIBLE_H

#include "ViewEffect.h"

/**
 * Draw nothing.
 */
class EffectInvisible : public ViewEffect {
    public:
        virtual bool isInvisible() const { return true; }
        virtual void blit(SDL_Surface *, SDL_Surface *, int, int) {}
};

#endif
