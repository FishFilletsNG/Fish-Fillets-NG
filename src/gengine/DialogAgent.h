#ifndef HEADER_DIALOGAGENT_H
#define HEADER_DIALOGAGENT_H

class Dialog;
class ResDialogPack;
class PlannedDialog;

#include "BaseAgent.h"
#include "Name.h"

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
    t_running m_cycling;
    private:
    void removeFirstNotTalking();
    bool isTalkingIn(int actor, const t_running &fifo) const;
    void killSoundIn(int actor, t_running &fifo);
    void killTalksIn(t_running &fifo);
    protected:
    virtual void own_init();
    virtual void own_update();
    virtual void own_shutdown();
    public:
    void addDialog(const std::string &name, Dialog *dialog);

    void actorTalk(int actor, const std::string &name,
            int volume, int loops=0);
    bool isTalking(int actor) const;
    void killSound(int actor);

    bool empty();
    void killTalks();
    void removeAll();
};

#endif

