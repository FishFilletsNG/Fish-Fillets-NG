#ifndef HEADER_VBOX_H
#define HEADER_VBOX_H

#include "WiBox.h"

/**
 * Vertical box.
 */
class VBox : public WiBox {
    private:
        int m_w;
        int m_h;
    public:
        VBox();
        virtual int getW() const { return m_w; }
        virtual int getH() const { return m_h; }
        virtual void addWidget(IWidget *new_widget);
        virtual void setShift(const V2 &shift);
};

#endif
