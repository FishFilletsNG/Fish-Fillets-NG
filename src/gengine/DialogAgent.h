#ifndef HEADER_DIALOGAGENT_H
#define HEADER_DIALOGAGENT_H

class Dialog;
class ResDialogPack;
class PlannedDialog;

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include <list>

/**
 * NPC dialogs.
 */
class DialogAgent : public BaseAgent {
    AGENT(DialogAgent, Name::DIALOG_NAME);
    private:
    ResDialogPack *m_dialogs;

    typedef std::list<PlannedDialog*> t_running;
    t_running m_running;
    private:
    void removeFirstNotTalking();
    protected:
    virtual void own_init();
    virtual void own_update();
    virtual void own_shutdown();
    public:
    void addDialog(const std::string &name, Dialog *dialog);

    void actorTalk(int actor, const std::string &name,
            int volume, int loops=0);
    bool isTalking(int actor);
    void killSound(int actor);

    bool empty();
    void killTalks();
    void removeAll();
};

#endif

