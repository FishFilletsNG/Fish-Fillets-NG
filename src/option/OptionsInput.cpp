/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "OptionsInput.h"

#include "MenuOptions.h"

//-----------------------------------------------------------------
OptionsInput::OptionsInput(MenuOptions *menu)
    : StateInput(menu)
{
}
//-----------------------------------------------------------------
MenuOptions *
OptionsInput::getMenu()
{
    return dynamic_cast<MenuOptions*>(m_state);
}
//-----------------------------------------------------------------
/**
 * Toggle menu.
 */
void
OptionsInput::enableMenu()
{
    quitState();
}
//-----------------------------------------------------------------
void
OptionsInput::mouseEvent(const MouseStroke &buttons)
{
    getMenu()->mouseButton(buttons);
}

