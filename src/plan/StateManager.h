#ifndef HEADER_STATEMANAGER_H
#define HEADER_STATEMANAGER_H

class GameState;

#include "NoCopy.h"

#include <vector>

/**
 * Stack of states.
 */
class StateManager : public NoCopy {
    private:
        typedef std::vector<GameState*> t_states;
        t_states m_states;
        t_states m_trash;
    private:
        void emptyTrash();
        void removeCurrent();
        void resumeStack();
        void resumeBg(t_states::iterator state);
        void pauseRunning();
    public:
        virtual ~StateManager();
        void updateGame();

        void changeState(GameState *new_state);
        void pushState(GameState *new_state);
        void popState();
};

#endif
