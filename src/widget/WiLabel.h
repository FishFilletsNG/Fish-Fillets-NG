#ifndef HEADER_WILABEL_H
#define HEADER_WILABEL_H

class Font;

#include "WiPicture.h"

#include "SDL.h"
#include <string>

/**
 * Constant text label.
 */
class WiLabel : public WiPicture {
    public:
        WiLabel(const std::string &text, const Font &font,
                const SDL_Color &color);
};

#endif
