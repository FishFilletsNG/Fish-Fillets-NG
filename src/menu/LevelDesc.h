#ifndef HEADER_LEVELDESC_H
#define HEADER_LEVELDESC_H

#include "Dialog.h"

/**
 * Multi lang. level description.
 */
class LevelDesc : public Dialog {
    private:
        std::string m_levelname;
    public:
        LevelDesc(const std::string &lang,
                const std::string &levelname,
                const std::string &desc);
        std::string getLevelName() const { return m_levelname; }
        std::string getDesc() const { return getSubtitle(); }
};

#endif

