#ifndef HEADER_SUBTITLEAGENT_H
#define HEADER_SUBTITLEAGENT_H

class ResFontPack;
class Title;

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include "SFont.h"

#include <string>
#include <deque>

/**
 * Subtitles manager.
 */
class SubTitleAgent : public BaseAgent {
    AGENT(SubTitleAgent, Name::SUBTITLE_NAME);
    public:
    static const int TITLE_ROW = 26;
    static const int TITLE_BASE = 20;
    static const int TITLE_SPEED = 2;
    static const int TITLE_BORDER = 20;
    static const int TITLE_LIMIT_Y = TITLE_BASE + 5 * TITLE_ROW;
    typedef std::deque<Title*> t_titles;
    t_titles m_titles;

    ResFontPack *m_fonts;
    private:
    std::string splitAndCreate(const std::string &subtitle, SFont_Font *font);
    void trimRest(std::string &buffer);
    void newShortSubtitle(const std::string &subtitle, SFont_Font *font);

    void shiftTitlesUp(int rate);
    void shiftFinalsUp(int rate);
    int lowestY();
    protected:
    virtual void own_init();
    virtual void own_update();
    virtual void own_shutdown();
    public:
    void addFont(const std::string &fontname, const Path &file);
    void newSubtitle(const std::string &subtitle, const std::string &fontname);

    void killTalk();
    void removeAll();
};

#endif

