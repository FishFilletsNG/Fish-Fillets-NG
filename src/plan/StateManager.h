#ifndef HEADER_STATEMANAGER_H
#define HEADER_STATEMANAGER_H

class GameState;

#include "NoCopy.h"

#include <list>

/**
 * Stack of states.
 */
class StateManager : public NoCopy {
    private:
        typedef std::list<GameState*> t_states;
        t_states m_states;
        t_states m_trash;
    private:
        void emptyTrash();
        void removeState(GameState *state);
        void insertAfter(GameState *who, GameState *new_state);
        t_states::iterator findIter(GameState *who);
        void checkStack();
        void pauseBg(t_states::iterator stateIt);
        void resumeBg(t_states::iterator stateIt);
        void installHandlers();
    public:
        virtual ~StateManager();
        void updateGame();

        void changeState(GameState *who, GameState *new_state);
        void pushState(GameState *who, GameState *new_state);
        void popState(GameState *who);
};

#endif
