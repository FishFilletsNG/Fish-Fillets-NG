#ifndef HEADER_RESIMAGEPACK_H
#define HEADER_RESIMAGEPACK_H

class Path;

#include "ResourcePack.h"
#include "ResCache.h"

#include "SDL.h"

/**
 * Image resources and image loading.
 */
class ResImagePack : public ResourcePack<SDL_Surface*> {
    private:
        static ResCache<SDL_Surface*> *CACHE;
        bool m_caching_enabled;
    public:
        explicit ResImagePack(bool caching_enabled=true);
        virtual const char *getName() const { return "image_pack"; }

        static SDL_Surface *loadImage(const Path &file);
        void addImage(const std::string &name, const Path &file);
        virtual void unloadRes(SDL_Surface *res);
};

#endif

