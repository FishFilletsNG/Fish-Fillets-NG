#ifndef HEADER_WIPARA_H
#define HEADER_WIPARA_H

class Font;

#include "VBox.h"

#include "SDL.h"
#include <string>

/**
 * Multi line paragraph.
 */
class WiPara : public VBox {
    public:
        WiPara(const std::string &text, const Font &font,
                const SDL_Color &color);
};

#endif
