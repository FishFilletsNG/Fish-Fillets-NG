#ifndef HEADER_WIBOX_H
#define HEADER_WIBOX_H

#include "IWidget.h"

#include <vector>

/**
 * Box which care about subwidget layout.
 * NOTE: soubwidgets should have constant size.
 */
class WiBox : public IWidget {
    protected:
        typedef std::vector<IWidget*> t_widgets;
        t_widgets m_widgets;
        virtual std::string own_getTip(const V2 &loc);
        virtual void own_mouseButton(const MouseStroke &stroke);
    public:
        virtual ~WiBox();
        virtual void addWidget(IWidget *new_widget) = 0;

        virtual void drawOn(SDL_Surface *screen);
};

#endif
