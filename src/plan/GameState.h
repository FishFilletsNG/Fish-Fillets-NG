#ifndef HEADER_GAMESTATE_H
#define HEADER_GAMESTATE_H

class StateManager;

#include "INamed.h"
#include "NoCopy.h"

/**
 * Game state.
 */
class GameState : public INamed, public NoCopy {
    private:
        bool m_active;
    protected:
        StateManager *m_manager;
    protected:
        virtual void own_initState() = 0;
        virtual void own_updateState() = 0;
        virtual void own_pauseState() = 0;
        virtual void own_resumeState() = 0;
        virtual void own_cleanState() = 0;
    public:
        void initState(StateManager *manager);
        void updateState();
        void pauseState();
        void resumeState();
        void cleanState();

        bool isRunning() const { return m_active; }
        virtual void quitState();
};

#endif
