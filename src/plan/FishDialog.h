#ifndef HEADER_FISHDIALOG_H
#define HEADER_FISHDIALOG_H

#include "Dialog.h"

#include <string>

/**
 * Wavy dialog.
 */
class FishDialog : public Dialog {
    private:
        std::string m_fontname;
    public:
        FishDialog(const std::string &lang,
                const std::string &soundfile, const std::string &subtitle,
                const std::string &fontname);
        virtual void runSubtitle(const StringTool::t_args &args) const;
};

#endif

