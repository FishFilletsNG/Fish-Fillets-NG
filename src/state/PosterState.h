#ifndef HEADER_POSTERSTATE_H
#define HEADER_POSTERSTATE_H

class Path;
class Picture;

#include "GameState.h"

/**
 * Static picture.
 */
class PosterState : public GameState {
    private:
        Picture *m_bg;
        GameState *m_nextState;
    protected:
        virtual void own_initState();
        virtual void own_updateState() {}
        virtual void own_pauseState() {}
        virtual void own_resumeState() {}
        virtual void own_cleanState() {}
    public:
        PosterState(const Path &picture);
        virtual ~PosterState();
        virtual const char *getName() const { return "state_poster"; };

        void setNextState(GameState *nextState) { m_nextState = nextState; }
        virtual void quitState();
};

#endif
