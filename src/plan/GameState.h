#ifndef HEADER_GAMESTATE_H
#define HEADER_GAMESTATE_H

class StateManager;
class InputHandler;

#include "INamed.h"
#include "NoCopy.h"

/**
 * Game state.
 * GameState installs own InputHandler in init phase.
 *
 * To change state,
 * use m_manager->pushState(topState) or
 * m_manager->changeState(newState)
 */
class GameState : public INamed, public NoCopy {
    private:
        bool m_active;
        InputHandler *m_handler;
    protected:
        StateManager *m_manager;
    protected:
        void takeHandler(InputHandler *new_handler);

        virtual void own_initState() = 0;
        virtual void own_updateState() = 0;
        virtual void own_pauseState() = 0;
        virtual void own_resumeState() = 0;
        virtual void own_cleanState() = 0;
    public:
        GameState();
        virtual ~GameState();

        void initState(StateManager *manager);
        void updateState();
        void pauseState();
        void resumeState();
        void cleanState();

        bool isRunning() const { return m_active; }
        virtual void quitState();
};

#endif
