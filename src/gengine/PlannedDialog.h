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
        const Dialog *m_dialog;
        int m_channel;
        int m_endtime;
        int m_minTime;
    private:
        bool isPlaying() const;
    public:
        PlannedDialog(int actor, const Dialog *dialog, int minTime);

        void talk(int volume, int loops=0);
        void killTalk();

        bool isTalking() const;
        bool equalsActor(int other) const;
};

#endif

