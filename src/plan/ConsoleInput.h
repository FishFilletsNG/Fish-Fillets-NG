#ifndef HEADER_CONSOLEINPUT_H
#define HEADER_CONSOLEINPUT_H

class KeyConsole;

#include "StateInput.h"

#include <string>

/**
 * Debug console input handler.
 */
class ConsoleInput : public StateInput {
    private:
        static const int KEY_HISTORY = 101;
        static const int KEY_BACKSPACE = 102;
        static const int KEY_CLEAR = 103;
        static const int KEY_ENTER = 104;
        std::string m_history;
    protected:
        virtual void enableConsole();
        virtual void enableSubtitles() {}
        virtual void specStroke(const KeyStroke &stroke);
        virtual void specKey(int keyIndex);
    private:
        KeyConsole *getConsole();
    public:
        ConsoleInput(KeyConsole *console);
};

#endif
