#ifndef HEADER_EFFECTZX_H
#define HEADER_EFFECTZX_H

#include "ViewEffect.h"

/**
 * Blinking like ZX spectrum loading.
 */
class EffectZx : public ViewEffect {
    private:
        enum eCorner {
            ZX1 = 1,
            ZX2 = 2,
            ZX3 = 3,
            ZX4 = 4
        };
        static const double STRIPE_STANDARD;
        static const double STRIPE_NARROW;
        int m_zx;
        int m_phase;
        double m_countHeight;
        double m_stripeHeight;
    public:
        EffectZx();
        static const char *NAME;
        virtual const char* getName() const { return NAME; }
        virtual void updateEffect();
        virtual void blit(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
};

#endif
