#ifndef HEADER_DIALOGAGENT_H
#define HEADER_DIALOGAGENT_H

class Dialog;
class ResDialogPack;
class PlannedDialog;

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include "SDL_mixer.h"

#include <deque>
#include <list>

/**
 * NPC dialogs.
 */
class DialogAgent : public BaseAgent {
    AGENT(DialogAgent, Name::DIALOG_NAME);
    private:
    ResDialogPack *m_dialogs;

    typedef std::deque<PlannedDialog*> t_planned;
    t_planned m_planned;
    typedef std::list<PlannedDialog*> t_running;
    t_running m_running;
    private:
    void removeFirstNotTalking();
    bool killOnePlanned(int actor);
    protected:
    virtual void own_init();
    virtual void own_update();
    virtual void own_shutdown();
    public:
    void addDialog(const std::string &name, Dialog *dialog);
    void planDialog(const std::string &name, int delay, int actor);
    bool isTalking(int actor);

    void killSound(int actor);
    void removeAll();
};

#endif

