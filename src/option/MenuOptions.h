#ifndef HEADER_MENUOPTIONS_H
#define HEADER_MENUOPTIONS_H

class Picture;
class MouseStroke;
class IWidget;

#include "GameState.h"

/**
 * Options menu which allow to set lang and tune volume.
 */
class MenuOptions : public GameState {
    private:
        IWidget *m_container;
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState() {}
        virtual void own_resumeState();
        virtual void own_cleanState() {}
    public:
        MenuOptions();
        virtual ~MenuOptions();
        virtual const char *getName() const { return "state_options"; };
        virtual bool allowBg() const { return true; }

        void mouseButton(const MouseStroke &stroke);
};

#endif