/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResDialogPack.h"

#include "Log.h"
#include "Dialog.h"
#include "OptionAgent.h"
#include "minmax.h"

//-----------------------------------------------------------------
    void
ResDialogPack::unloadRes(Dialog *res)
{
    delete res;
}
//-----------------------------------------------------------------
/**
 * Return match score.
 * One string must excatly be starting substring of the other.
 */
int
ResDialogPack::matchScore(const std::string &first,
        const std::string &second) const
{
    int score = 0;
    int minSize = min(first.size(), second.size());
    for (int i = 0; i < minSize; ++i) {
        if (first[i] == second[i]) {
            score++;
        }
        else {
            return 0;
        }
    }
    return score;
}
//-----------------------------------------------------------------
/**
 * Return dialog or NULL.
 * Compare dialog names and lang codes.
 * The best lang match is selected, at least two characters must equals.
 */
   const Dialog *
ResDialogPack::findDialog(const std::string &name,
        const std::string &lang)
{
    int bestScore = 0;
    Dialog *bestDialog = NULL;

    t_range range = getRange(name);
    t_range::iterator end = range.end();
    for (t_range::iterator i = range.begin(); i != end; ++i) {
        int score = matchScore(lang, (*i)->getLang());
        if (bestScore < score) {
            bestScore = score;
            bestDialog = *i;
        }
    }

    if (bestScore < 2) {
        bestDialog = NULL;
    }
    return bestDialog;
}
//-----------------------------------------------------------------
/**
 * Try find dialog for current lang or default lang.
 * @return dialog or NULL
 */
    const Dialog *
ResDialogPack::findDialogHard(const std::string &name)
{
    std::string lang = OptionAgent::agent()->getParam("lang");
    const Dialog *dialog = findDialog(name, lang);
    if (NULL == dialog) {
        dialog = findDialog(name, Dialog::DEFAULT_LANG);
        if (NULL == dialog) {
            LOG_WARNING(ExInfo("cannot find dialog")
                    .addInfo("name", name)
                    .addInfo("lang", lang)
                    .addInfo("pack", toString()));
        }
    }

    return dialog;
}
//-----------------------------------------------------------------
/**
 * Try find dialog for lang=speech or default lang.
 * @return dialog or NULL
 */
    const Dialog *
ResDialogPack::findDialogSpeech(const std::string &name)
{
    std::string speech = OptionAgent::agent()->getParam("speech",
            OptionAgent::agent()->getParam("lang"));
    const Dialog *dialog = findDialog(name, speech);
    if (NULL == dialog || dialog->isSpeechless()) {
        dialog = findDialog(name, Dialog::DEFAULT_LANG);
        if (NULL == dialog) {
            LOG_WARNING(ExInfo("cannot find speech")
                    .addInfo("name", name)
                    .addInfo("speech", speech)
                    .addInfo("pack", toString()));
        }
    }

    return dialog;
}

