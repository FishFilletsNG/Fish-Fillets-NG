#ifndef HEADER_RESDIALOGPACK_H
#define HEADER_RESDIALOGPACK_H

class Dialog;

#include "ResourcePack.h"
#include "Name.h"
#include "Path.h"

#include "SDL.h"

/**
 * Multi-language dialogs pack.
 */
class ResDialogPack : public ResourcePack<Dialog*> {
    protected:
        virtual void unloadRes(Dialog *res);
    public:
        virtual const char *getName() const { return "dialog_pack"; }
        Dialog *findDialog(const std::string &name, const std::string lang);
};

#endif

