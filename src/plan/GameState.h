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
        bool m_onBg;
        InputHandler *m_handler;
        StateManager *m_manager;
    protected:
        void takeHandler(InputHandler *new_handler);

        virtual void own_initState() = 0;
        virtual void own_updateState() = 0;
        virtual void own_pauseState() = 0;
        virtual void own_resumeState() = 0;
        virtual void own_cleanState() = 0;

        virtual void own_noteBg() {}
        virtual void own_noteFg() {}

        void changeState(GameState *new_state);
    public:
        GameState();
        virtual ~GameState();
        bool isRunning() const { return m_active; }
        bool isOnBg() const { return m_onBg; }
        virtual bool allowBg() const { return false; }

        void initState(StateManager *manager);
        void updateState();
        void pauseState();
        void resumeState();
        void cleanState();

        void quitState();
        void pushState(GameState *new_state);

        void noteBg();
        void noteFg();
};

#endif
