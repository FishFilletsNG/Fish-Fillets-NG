#ifndef HEADER_PLANNEDDIALOG_H
#define HEADER_PLANNEDDIALOG_H

class Dialog;

#include "NoCopy.h"

#include <string>

/**
 * Active dialog.
 */
class PlannedDialog : public NoCopy {
    private:
        int m_actor;
        Dialog *m_dialog;
        int m_channel;
        int m_endtime;
    private:
        bool isPlaying();
    public:
        PlannedDialog(int actor, Dialog *dialog);

        void talk();
        void killTalk();

        bool isTalking();
        bool equalsActor(int other) const;
};

#endif

