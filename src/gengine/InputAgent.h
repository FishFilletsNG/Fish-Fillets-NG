#ifndef HEADER_INPUTAGENT_H
#define HEADER_INPUTAGENT_H

class KeyBinder;
class RectBinder;
class KeyConsole;
class InputHandler;

#include "BaseAgent.h"
#include "Name.h"

#include "SDL.h"

/**
 * Forward input events to handlers.
 */
class InputAgent : public BaseAgent {
    AGENT(InputAgent, Name::INPUT_NAME);
    private:
        Uint8 *m_keys;
        KeyBinder *m_keyBinder;
        RectBinder *m_rectBinder;
        KeyConsole *m_console;
        InputHandler *m_handler;
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        virtual void receiveSimple(const SimpleMsg *msg);
        void installHandler(InputHandler *handler) { m_handler = handler; }

        KeyBinder *keyBinder() { return m_keyBinder; }
        RectBinder *rectBinder() { return m_rectBinder; }
        Uint8 *getKeys() { return m_keys; }
};

#endif
