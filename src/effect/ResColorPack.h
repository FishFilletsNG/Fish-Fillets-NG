#ifndef HEADER_RESCOLORPACK_H
#define HEADER_RESCOLORPACK_H

#include "ResourcePack.h"
#include "Color.h"

/**
 * Pack of color aliases.
 */
class ResColorPack : public ResourcePack<Color*> {
    protected:
        virtual void unloadRes(Color *res) { delete res; }
    public:
        virtual const char *getName() const { return "color_pack"; }
};

#endif

