#ifndef HEADER_RESDIALOGPACK_H
#define HEADER_RESDIALOGPACK_H

class Dialog;

#include "ResourcePack.h"
#include "Name.h"

#include "SDL.h"

/**
 * Multi-language dialogs pack.
 */
class ResDialogPack : public ResourcePack<Dialog*> {
    private:
        int matchScore(const std::string &first,
                const std::string &second) const;
        const Dialog *findDialog(const std::string &name,
                const std::string &lang);
    public:
        virtual const char *getName() const { return "dialog_pack"; }
        const Dialog *findDialogHard(const std::string &name);
        const Dialog *findDialogSpeech(const std::string &name);
        virtual void unloadRes(Dialog *res);
};

#endif

