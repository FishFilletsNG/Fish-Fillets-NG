#ifndef HEADER_DIALOGSTACK_H
#define HEADER_DIALOGSTACK_H
#define HEADER_DIALOGAGENT_H

class Dialog;
class ResDialogPack;
class PlannedDialog;

#include "NoCopy.h"

#include <list>
#include <string>

/**
 * Stack of running dialogs.
 */
class DialogStack : public NoCopy {
    private:
        ResDialogPack *m_dialogs;

        typedef std::list<PlannedDialog*> t_running;
        t_running m_running;
        t_running m_cycling;
        PlannedDialog *m_activeDialog;
    private:
        void removeFirstNotTalking();
        bool isTalkingIn(int actor, const t_running &fifo) const;
        void killSoundIn(int actor, t_running &fifo);
        void killTalksIn(t_running &fifo);
        void releaseDialog(PlannedDialog *dialog);
    public:
        DialogStack();
        virtual ~DialogStack();
        void updateStack();

        void addDialog(const std::string &name, Dialog *dialog);
        void actorTalk(int actor, const std::string &name,
                int volume, int loops=0, bool dialogFlag=false);
        bool isTalking(int actor) const;
        void killSound(int actor);

        bool isDialog() const;
        bool areRunning() const { return !m_running.empty(); }
        void killTalks();
        void removeAll();
};

#endif

