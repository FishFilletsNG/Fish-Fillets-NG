/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "FishDialog.h"

#include "SubTitleAgent.h"

//-----------------------------------------------------------------
/**
 * Prepare new wavy dialoge.
 * @param lang lang
 * @param soundfile a wav file
 * @param subtitle subtitle
 * @param fontname name of selected font
 */
FishDialog::FishDialog(const std::string &lang,
        const Path &soundfile, const std::string &subtitle,
        const std::string &fontname)
: Dialog(lang, soundfile, subtitle), m_fontname(fontname)
{
    /* empty */
}
//-----------------------------------------------------------------
void
FishDialog::runSubtitle() const
{
    SubTitleAgent::agent()->newSubtitle(getSubtitle(), m_fontname);
}
