#ifndef HEADER_MOVIESTATE_H
#define HEADER_MOVIESTATE_H
#ifdef HAVE_SMPEG

class SDL_Movie;

#include "Path.h"
#include "GameState.h"

/**
 * A MPEG-1 movie.
 */
class MovieState : public GameState {
    protected:
        Path m_file;
        SDL_Movie *m_movie;
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState() {}
        virtual void own_resumeState() {}
        virtual void own_cleanState();
    public:
        MovieState(const Path &file);
        virtual const char *getName() const { return "state_movie"; };
};

#endif
#endif
