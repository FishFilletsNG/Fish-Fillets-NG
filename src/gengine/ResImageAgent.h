#ifndef HEADER_RESIMAGEAGENT_H
#define HEADER_RESIMAGEAGENT_H

#include "BaseAgent.h"
#include "ResImagePack.h"
#include "Name.h"
#include "Path.h"

#include "SDL.h"

/**
 * Image resources and image loading.
 */
class ResImageAgent : public BaseAgent {
    AGENT(ResImageAgent, Name::RESIMAGE_NAME);
    private:
    ResImagePack *m_pack;
    protected:
    virtual void own_shutdown();
    public:
    ResImageAgent();
    virtual ~ResImageAgent();

    SDL_Surface *loadImage(const Path &file) { return m_pack->loadImage(file); }
    void addImage(const std::string &name, const Path &file)
    {
        m_pack->addImage(name, file);
    }
};

#endif

