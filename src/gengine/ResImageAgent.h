#ifndef HEADER_RESIMAGEAGENT_H
#define HEADER_RESIMAGEAGENT_H

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"
#include "ResImagePack.h"

#include "SDL.h"

/**
 * Image resources and image loading.
 */
class ResImageAgent : public BaseAgent {
    AGENT(ResImageAgent, Name::RESIMAGE_NAME);
    private:
    ResImagePack *m_pack;
    protected:
    virtual void own_init();
    virtual void own_shutdown();
    public:
    SDL_Surface *loadImage(const Path &file) { return m_pack->loadImage(file); }
};

#endif

