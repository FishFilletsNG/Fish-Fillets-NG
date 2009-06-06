#ifndef HEADER_EFFECTNONE_H
#define HEADER_EFFECTNONE_H

#include "ViewEffect.h"

/**
 * Blit without any special effect.
 */
class EffectNone : public ViewEffect {
    public:
        static const char *NAME;
        virtual const char* getName() const { return NAME; }
        virtual void blit(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
};

#endif
