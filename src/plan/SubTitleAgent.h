#ifndef HEADER_SUBTITLEAGENT_H
#define HEADER_SUBTITLEAGENT_H

class ResColorPack;
class Title;
class Font;
class Color;

#include "BaseAgent.h"
#include "Drawable.h"
#include "Name.h"

#include <string>
#include <deque>

/**
 * Subtitles manager.
 */
class SubTitleAgent : public BaseAgent, public Drawable {
    AGENT(SubTitleAgent, Name::SUBTITLE_NAME);
    public:
    static const int TITLE_ROW = 26;
    static const int TITLE_BASE = 20;
    static const int TITLE_SPEED = 2;
    static const int TITLE_BORDER = 20;
    static const int TITLE_LIMIT_Y = TITLE_BASE + 5 * TITLE_ROW;
    typedef std::deque<Title*> t_titles;
    t_titles m_titles;

    Font *m_font;
    ResColorPack *m_colors;
    int m_limitY;
    private:
    std::string splitAndCreate(const std::string &subtitle, const Color *color);
    void trimRest(std::string &buffer);
    void newShortSubtitle(const std::string &subtitle, const Color *color);

    void shiftTitlesUp(int rate);
    void shiftFinalsUp(int rate);
    int lowestY();
    protected:
    virtual void own_init();
    virtual void own_update();
    virtual void own_shutdown();
    public:
    int getLimitY() const { return m_limitY; }
    void setLimitY(int limitY) { m_limitY = limitY; }
    void addFont(const std::string &fontname, Color *new_color);
    void newSubtitle(const std::string &subtitle, const std::string &fontname);

    void killTalks();
    void removeAll();

    virtual void drawOn(SDL_Surface *screen);
};

#endif

