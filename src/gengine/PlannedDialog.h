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
        const Dialog *m_dialog;
        int m_channel;
        bool m_busy;
        bool m_running;
    public:
        PlannedDialog(Actor *actor, int delay, const Dialog *dialog,
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

