/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PedoInput.h"

#include "Pedometer.h"
#include "Keymap.h"

#include "KeyStroke.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
PedoInput::PedoInput(Pedometer *pedometer)
    : GameInput(pedometer)
{
}
//-----------------------------------------------------------------
Pedometer *
PedoInput::getPedo()
{
    return dynamic_cast<Pedometer*>(m_state);
}
//-----------------------------------------------------------------
void
PedoInput::mouseEvent(const MouseStroke &buttons)
{
    if (buttons.isLeft()) {
        getPedo()->runSelected();
    }
}

