#ifndef HEADER_LAYEREDPICTURE_H
#define HEADER_LAYEREDPICTURE_H

#include "Picture.h"

#include "SDL.h"

/**
 * Picture with two layers and color mask.
 */
class LayeredPicture : public Picture {
    private:
        static const Uint32 MASK_NO = static_cast<Uint32>(-1);
        SDL_Surface *m_lowerLayer;
        SDL_Surface *m_colorMask;
        Uint32 m_activeColor;
    public:
        LayeredPicture(const Path &bg_file, const V2 &loc,
                const Path &lowerLayer, const Path &colorMask);
        ~LayeredPicture();

        void setNoActive() { m_activeColor = MASK_NO; }
        void setActiveMask(Uint32 color) { m_activeColor = color; }

        Uint32 getMaskAtWorld(const V2 &worldLoc);
        Uint32 getMaskAt(const V2 &loc);
        Uint32 getNoMask() const { return MASK_NO; }

        virtual void drawOn(SDL_Surface *screen);
};

#endif
