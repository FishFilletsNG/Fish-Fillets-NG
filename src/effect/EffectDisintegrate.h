#ifndef HEADER_EFFECTDISINTEGRATE_H
#define HEADER_EFFECTDISINTEGRATE_H

#include "ViewEffect.h"

/**
 * Draw disintegrating skeleton.
 */
class EffectDisintegrate : public ViewEffect {
    private:
        static const int DISINT_START = 400;
        static const int DISINT_SPEED = 30;
        int m_disint;
    public:
        EffectDisintegrate();
        static const char *NAME;
        virtual const char* getName() const { return NAME; }
        virtual void updateEffect();
        virtual bool isDisintegrated() const;
        virtual bool isInvisible() const;
        virtual void blit(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
};

#endif
