#ifndef HEADER_SELECTLANG_H
#define HEADER_SELECTLANG_H

class LangFlag;
class MouseStroke;
class Path;
class BaseMsg;
class StringMsg;
class RectBinder;

#include "IDrawer.h"
#include "BaseListener.h"
#include "V2.h"

#include <vector>
#include <string>

/**
 * Menu with flags to select prefered lang.
 */
class SelectLang : public IDrawer, public BaseListener {
    private:
        static const int MAX_WIDTH = 300;
        static const int FLAG_MARGIN = 5;
        typedef std::vector<LangFlag*> t_flags;
        t_flags m_flags;
        V2 m_shift;
        int m_flagW;
        int m_flagH;
        RectBinder *m_rectBinder;
    private:
        int flagsPerRow() const;
        int flagsPerCol() const;
        int selectedW() const { return m_flagW + 2 * FLAG_MARGIN; }
        int selectedH() const { return m_flagH + 2 * FLAG_MARGIN; }
        void registerRects();
        BaseMsg *createSetMsg(const std::string &lang);
        void drawSelected(const V2 &loc);
    public:
        SelectLang(const Path &datafile);
        virtual ~SelectLang();
        virtual const char *getName() const { return "select_lang"; }
        int getW() const;
        int getH() const;

        void addFlag(LangFlag *new_flag);
        void initFlags();

        virtual void draw();
        void setShift(const V2 &shift) { m_shift = shift; }
        void mouseButton(const MouseStroke &stroke);

        virtual void receiveString(const StringMsg *msg);
};

#endif
