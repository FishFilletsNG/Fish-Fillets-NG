#ifndef HEADER_SELECTLANG_H
#define HEADER_SELECTLANG_H

class Path;

#include "VBox.h"
#include "Scripter.h"

/**
 * Menu with flags to select prefered lang.
 */
class SelectLang : public VBox, public Scripter {
    private:
        static const int MAX_WIDTH = 200;
        std::string m_option;
        WiBox *m_activeRow;
    public:
        SelectLang(const std::string &option, const Path &datafile);

        void addFlag(const std::string &value, const Path &picture);
};

#endif
