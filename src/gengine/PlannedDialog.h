#ifndef HEADER_PLANNEDDIALOG_H
#define HEADER_PLANNEDDIALOG_H

class Dialog;
class Actor;
#include "NoCopy.h"

#include <string>

/**
 * Active dialog.
 */
class PlannedDialog: public NoCopy {
    private:
        Actor *m_actor;
        int m_delay;
        Dialog *m_dialog;
        int m_channel;
        bool m_busy;
        bool m_running;
        int m_endtime;
    private:
        bool isPlaying();
    public:
        PlannedDialog(Actor *actor, int delay, Dialog *dialog,
                bool busy);
        ~PlannedDialog();

        void talk();
        void killTalk();

        bool isTalking();
        bool equalsActor(const Actor *other) const;
        int getDelay() const { return m_delay; }
        void decDelay() { m_delay--; }
};

#endif

