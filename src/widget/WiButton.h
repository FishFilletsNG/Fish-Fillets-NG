#ifndef HEADER_WIBUTTON_H
#define HEADER_WIBUTTON_H

class BaseMsg;

#include "WiContainer.h"

/**
 * Button widget.
 */
class WiButton : public WiContainer {
    private:
        BaseMsg *m_msg;
    protected:
        virtual void own_mouseButton(const MouseStroke &stroke);
    public:
        WiButton(IWidget *new_content, BaseMsg *new_msg);
        virtual ~WiButton();
};

#endif
