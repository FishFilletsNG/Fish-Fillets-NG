/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "HBox.h"

#include "minmax.h"

//-----------------------------------------------------------------
/**
 * Starts as empty box.
 */
HBox::HBox()
{
    m_w = 0;
    m_h = 0;
}
//-----------------------------------------------------------------
/**
 * Place widget to the most right place.
 */
void
HBox::addWidget(IWidget *new_widget)
{
    m_widgets.push_back(new_widget);
    new_widget->setShift(m_shift.plus(V2(m_w, 0)));
    m_w += new_widget->getW();
    m_h = max(m_h, new_widget->getH());
}
//-----------------------------------------------------------------
/**
 * Update shift for all subwidgets.
 */
void
HBox::setShift(const V2 &shift)
{
    m_shift = shift;
    int width = 0;
    t_widgets::iterator end = m_widgets.end();
    for (t_widgets::iterator i = m_widgets.begin(); i != end; ++i) {
        (*i)->setShift(m_shift.plus(V2(width, 0)));
        width += (*i)->getW();
    }
}


