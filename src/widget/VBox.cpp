/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "VBox.h"

#include "minmax.h"

//-----------------------------------------------------------------
/**
 * Starts as empty box.
 */
VBox::VBox()
{
    m_w = 0;
    m_h = 0;
    m_centered = false;
}
//-----------------------------------------------------------------
/**
 * Place widget to the lowest vertical place.
 */
void
VBox::addWidget(IWidget *new_widget)
{
    m_widgets.push_back(new_widget);
    m_w = max(m_w, new_widget->getW());
    int shiftX = 0;
    if (m_centered) {
        shiftX = (m_w - new_widget->getW()) / 2;
    }
    new_widget->setShift(m_shift.plus(V2(shiftX, m_h)));
    m_h += new_widget->getH();
}
//-----------------------------------------------------------------
/**
 * Update shift for all subwidgets.
 */
void
VBox::setShift(const V2 &shift)
{
    m_shift = shift;
    int height = 0;
    t_widgets::iterator end = m_widgets.end();
    for (t_widgets::iterator i = m_widgets.begin(); i != end; ++i) {
        int shiftX = 0;
        if (m_centered) {
            shiftX = (m_w - (*i)->getW()) / 2;
        }
        (*i)->setShift(m_shift.plus(V2(shiftX, height)));
        height += (*i)->getH();
    }
}


