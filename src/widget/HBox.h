#ifndef HEADER_HBOX_H
#define HEADER_HBOX_H

#include "WiBox.h"

/**
 * Horizontal box.
 */
class HBox : public WiBox {
    private:
        int m_w;
        int m_h;
    public:
        HBox();
        virtual int getW() const { return m_w; }
        virtual int getH() const { return m_h; }
        virtual void addWidget(IWidget *new_widget);
        virtual void setShift(const V2 &shift);
};

#endif
