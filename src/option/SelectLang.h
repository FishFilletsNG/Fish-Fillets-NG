#ifndef HEADER_SELECTLANG_H
#define HEADER_SELECTLANG_H

class Path;

#include "VBox.h"

/**
 * Menu with flags to select prefered lang.
 */
class SelectLang : public VBox {
    private:
        static const int MAX_WIDTH = 300;
        WiBox *m_activeRow;
    public:
        SelectLang(const Path &datafile);

        void addFlag(const std::string &lang, const Path &picture);
};

#endif
