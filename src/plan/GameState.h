#ifndef HEADER_GAMESTATE_H
#define HEADER_GAMESTATE_H

class StateManager;
class InputHandler;
class InputProvider;
class MultiDrawer;
class Drawable;

#include "NoCopy.h"
#include "BaseListener.h"

/**
 * Game state.
 * GameState installs own InputHandler in init phase.
 *
 * To change state,
 * use m_manager->pushState(topState) or
 * m_manager->changeState(newState)
 */
class GameState : public BaseListener, public NoCopy {
    private:
        bool m_active;
        bool m_onBg;
        GameState *m_nextState;
        InputHandler *m_handler;
        MultiDrawer *m_drawer;
        StateManager *m_manager;
    protected:
        void takeHandler(InputHandler *new_handler);
        const InputProvider *getInput();

        virtual void own_initState() = 0;
        virtual void own_updateState() = 0;
        virtual void own_pauseState() = 0;
        virtual void own_resumeState() = 0;
        virtual void own_cleanState() = 0;

        virtual void own_noteBg() {}
        virtual void own_noteFg() {}

        void changeState(GameState *new_state);
        void registerDrawable(Drawable *drawable);
        void deregisterDrawable(const Drawable *drawable);
    public:
        GameState();
        virtual ~GameState();
        virtual bool allowBg() const { return false; }
        bool isRunning() const { return m_active; }
        bool isOnBg() const { return m_onBg; }
        void setNextState(GameState *nextState) { m_nextState = nextState; }

        void initState(StateManager *manager);
        void updateState();
        void pauseState();
        void resumeState();
        void cleanState();

        void quitState();
        void pushState(GameState *new_state);

        void noteBg();
        void noteFg();
        void installHandlers();
        void unHandlers();

        void receiveSimple(const SimpleMsg *msg);
};

#endif
