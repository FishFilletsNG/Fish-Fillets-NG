#ifndef HEADER_MENUHELP_H
#define HEADER_MENUHELP_H

class VBox;

#include "GameState.h"
#include "Drawable.h"

/**
 * Help screen.
 */
class MenuHelp : public GameState, public Drawable {
    private:
        VBox *m_help;
    private:
        void prepareText();
    protected:
        virtual void own_initState();
        virtual void own_updateState() {}
        virtual void own_pauseState() {}
        virtual void own_resumeState();
        virtual void own_cleanState() {}
    public:
        MenuHelp();
        virtual ~MenuHelp();
        virtual const char *getName() const { return "state_help"; };
        virtual bool allowBg() const { return true; }

        virtual void drawOn(SDL_Surface *screen);
};

#endif
