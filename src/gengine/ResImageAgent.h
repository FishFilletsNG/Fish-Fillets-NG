#ifndef HEADER_RESIMAGEAGENT_H
#define HEADER_RESIMAGEAGENT_H

#include "ResourceAgent.h"
#include "Name.h"
#include "Path.h"

#include "SDL.h"

/**
 * Image resources and image loading.
 */
class ResImageAgent : public ResourceAgent<SDL_Surface*> {
    AGENT(ResImageAgent, Name::RESIMAGE_NAME);
    protected:
    virtual void unloadRes(SDL_Surface *res);
    public:
    SDL_Surface *loadImage(const Path &file);
    void addImage(const std::string &name, const Path &file);
};

#endif

