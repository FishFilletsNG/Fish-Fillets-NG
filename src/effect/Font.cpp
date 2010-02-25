/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Font.h"

#include "Log.h"
#include "Path.h"
#include "TTFException.h"
#include "SDLException.h"
#include "Outline.h"

#ifdef HAVE_FRIBIDI
#include "fribidi.h"
#endif

std::string
Font::biditize(const std::string &text)
{
#ifdef HAVE_FRIBIDI
    FriBidiCharType base = FRIBIDI_TYPE_ON;
    FriBidiChar *logicalString = new FriBidiChar[text.length() + 1];
    FriBidiChar *visualString = new FriBidiChar[text.length() + 1];

    int ucsLength = fribidi_charset_to_unicode(FRIBIDI_CHAR_SET_UTF8,
            const_cast<char*>(text.c_str()),
            text.length(), logicalString);
    fribidi_boolean ok = fribidi_log2vis(logicalString, ucsLength, &base,
            visualString, NULL, NULL, NULL);
    if (!ok) {
        LOG_WARNING(ExInfo("cannot biditize text")
                .addInfo("text", text));
        return text;
    }

    char *buffer = new char[text.length() + 1];
    int length = fribidi_unicode_to_charset(FRIBIDI_CHAR_SET_UTF8,
            visualString, ucsLength, buffer);
    std::string result = std::string(buffer, length);
    delete[] buffer;
    delete[] visualString;
    delete[] logicalString;
    return result;
#else
    return text;
#endif
}

//-----------------------------------------------------------------
/**
 * Create new font from file.
 * Initialized TTF_Init when necessary.
 * @param file_ttf path to ttf file
 * @param height font height
 * @throws TTFException when cannot open font
 */
Font::Font(const Path &file_ttf, int height)
{
    m_ttfont = TTF_OpenFont(file_ttf.getNative().c_str(), height);
    if (!m_ttfont) {
        throw TTFException(ExInfo("OpenFont")
                .addInfo("file", file_ttf.getNative()));
    }

    //NOTE: bg color will be set to be transparent
    SDL_Color bg = {10, 10, 10, 0};
    m_bg = bg;
}
//-----------------------------------------------------------------
Font::~Font()
{
    TTF_CloseFont(m_ttfont);
}
//-----------------------------------------------------------------
/**
 * Prepare font rendering.
 * @throws TTFException when cannot init SDL_ttf.
 */
void
Font::init()
{
    if (TTF_Init() < 0) {
        throw TTFException(ExInfo("Init"));
    }
}
//-----------------------------------------------------------------
/**
 * Deinit font subsystem.
 */
void
Font::shutdown()
{
    TTF_Quit();
}

//-----------------------------------------------------------------
    int
Font::calcTextWidth(const std::string &text)
{
    int w;
    TTF_SizeUTF8(m_ttfont, text.c_str(), &w, NULL);
    return w;
}
//-----------------------------------------------------------------
/**
 * Render text with this color.
 * @param text utf-8 encoded text
 * @param color text color
 * @return new rendered surface
 * @throws TTFException when render fails
 * @throws SDLException when converting fails
 */
SDL_Surface *
Font::renderText(const std::string &text, const SDL_Color &color) const
{
    std::string content = biditize(text);
    if (text.empty()) {
        content = " ";
        LOG_WARNING(ExInfo("empty text to render")
                .addInfo("r", color.r)
                .addInfo("g", color.g)
                .addInfo("b", color.b));
    }

    SDL_Surface *raw_surface = TTF_RenderUTF8_Shaded(m_ttfont, content.c_str(),
            color, m_bg);
    if (!raw_surface) {
        throw TTFException(ExInfo("RenderUTF8")
                .addInfo("text", text));
    }

    //NOTE: at index 0 is bg color
    if (SDL_SetColorKey(raw_surface, SDL_SRCCOLORKEY, 0) < 0) {
        throw SDLException(ExInfo("SetColorKey"));
    }

    SDL_Surface *surface = SDL_DisplayFormat(raw_surface);
    if (!surface) {
        throw SDLException(ExInfo("DisplayFormat"));
    }
    SDL_FreeSurface(raw_surface);

    return surface;
}
//-----------------------------------------------------------------
/**
 * Render text with black outline around font.
 * @param text utf-8 encoded text
 * @param color text color
 * @param outlineWidth outline width
 * @return new rendered surface
 */
SDL_Surface *
Font::renderTextOutlined(const std::string &text,
                const SDL_Color &color, int outlineWidth) const
{
    static const SDL_Color BLACK = {0, 0, 0, 255};
    //NOTE: uses spaces to ensure space for outline
    SDL_Surface *surface = renderText(" " + text + " ", color);
    Outline outline(BLACK, outlineWidth);

    outline.drawOnColorKey(surface);
    return surface;
}

