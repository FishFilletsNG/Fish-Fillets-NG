#ifndef HEADER_RESSOUNDAGENT_H
#define HEADER_RESSOUNDAGENT_H

class ResSoundPack;

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include "SDL_mixer.h"

/**
 * Sound resources.
 */
class ResSoundAgent : public BaseAgent {
    AGENT(ResSoundAgent, Name::RESSOUND_NAME);
    private:
    ResSoundPack *m_pack;
    protected:
    virtual void own_init();
    virtual void own_shutdown();
    public:
    Mix_Chunk *loadSound(const Path &file);
};

#endif

