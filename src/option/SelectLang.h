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
        static const int MAX_WIDTH = 260;
        WiBox *m_activeRow;
    public:
        SelectLang(const Path &datafile);

        void addFlag(const std::string &lang, const Path &picture);
};

#endif
