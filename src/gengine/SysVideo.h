#ifndef HEADER_SYSVIDEO_H
#define HEADER_SYSVIDEO_H

#include <string>

/**
 * System dependend video functions.
 */
class SysVideo {
    public:
        static void setCaption(const std::string &title);
};

#endif
