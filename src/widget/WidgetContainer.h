#ifndef HEADER_WIDGETCONTAINER_H
#define HEADER_WIDGETCONTAINER_H

#include "IDrawer.h"
#include "IWidget.h"

/**
 * Drawer with widget inside.
 */
class WidgetContainer : public IDrawer {
    private:
        IWidget *m_widget;
    public:
        WidgetContainer(IWidget *new_widget);
        virtual ~WidgetContainer();
        int getW() const { return m_widget->getW(); }
        int getH() const { return m_widget->getH(); }
        void setShift(const V2 &shift) { m_widget->setShift(shift); }

        virtual void draw();
        void mouseButton(const MouseStroke &stroke);
};

#endif
