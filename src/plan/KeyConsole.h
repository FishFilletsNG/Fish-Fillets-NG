#ifndef HEADER_KEYCONSOLE_H
#define HEADER_KEYCONSOLE_H

class Font;

#include "GameState.h"
#include "Drawable.h"
#include "Color.h"

#include <string>
#include "SDL.h"

/**
 * Debug console.
 */
class KeyConsole : public GameState, public Drawable {
    private:
        Font *m_font;
        Color m_color;
        std::string m_input;
    protected:
        virtual void own_initState() {};
        virtual void own_updateState() {};
        virtual void own_pauseState() {};
        virtual void own_resumeState() {};
        virtual void own_cleanState() {};
    public:
        KeyConsole();
        virtual ~KeyConsole();
        virtual const char *getName() const { return "state_console"; };
        virtual bool allowBg() const { return true; }

        void setInput(const std::string &input) { m_input = input; }
        std::string getInput() const { return m_input; }

        bool sendCommand();
        virtual void drawOn(SDL_Surface *screen);
};

#endif
