#ifndef HEADER_FINDERALG_H
#define HEADER_FINDERALG_H

class V2;
class Unit;

#include "Dir.h"
#include "FinderField.h"
#include "FinderPlace.h"

#include <deque>

/**
 * Algorithm to find shortest path.
 */
class FinderAlg {
    private:
        const Unit *m_unit;
        FinderField m_closed;
        std::deque<FinderPlace> m_fifo;
    private:
        void pushNext(const FinderPlace &parent, const V2 &shift);
        bool isInRect(const V2 &rectLoc, int w, int h, const V2 &dest) const;
        bool tryPlace(const FinderPlace &place) const;
    public:
        FinderAlg(int w, int h);
        Dir::eDir findDir(const Unit *unit, const V2 &dest);
};

#endif
