#ifndef HEADER_RESFONTPACK_H
#define HEADER_RESFONTPACK_H

#include "ResourcePack.h"
#include "Path.h"

#include "SFont.h"

/**
 * Pack of fonts.
 */
class ResFontPack : public ResourcePack<SFont_Font*> {
    protected:
    virtual void unloadRes(SFont_Font *res);
    public:
    SFont_Font *loadFont(const Path &file);
    void addFont(const std::string &name, const Path &file);
};

#endif

