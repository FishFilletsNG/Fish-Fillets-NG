#include "ResDialogPack.h"

#include "Log.h"
#include "Dialog.h"

//-----------------------------------------------------------------
    void
ResDialogPack::unloadRes(Dialog *res)
{
    delete res;
}
//-----------------------------------------------------------------
/**
 * Return dialog or NULL.
 */
    Dialog *
ResDialogPack::findDialog(const std::string &name, const std::string lang)
{
    t_range range = getRange(name);
    t_range::iterator end = range.end();
    for (t_range::iterator i = range.begin(); i != end; ++i) {
        if ((*i)->getLang() == lang) {
            return *i;
        }
    }

    LOG_WARNING(ExInfo("cannot find dialog")
            .addInfo("name", name)
            .addInfo("lang", lang));
    return NULL;
}

