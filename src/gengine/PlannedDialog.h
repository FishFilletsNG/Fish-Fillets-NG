#ifndef HEADER_PLANNEDDIALOG_H
#define HEADER_PLANNEDDIALOG_H

class Dialog;
#include "NoCopy.h"

#include <string>

/**
 * Active dialog.
 */
class PlannedDialog: public NoCopy {
    private:
        int m_actor;
        int m_delay;
        const Dialog *m_dialog;
        int m_channel;
    public:
        PlannedDialog(int actor, int delay, const Dialog *dialog);

        void talk();
        void killTalk();

        bool isTalking();
        int getActor() const { return m_actor; }
        int getDelay() const { return m_delay; }
        void decDelay() { m_delay--; }
};

#endif

