#ifndef HEADER_FINDERPLACE_H
#define HEADER_FINDERPLACE_H

#include "Dir.h"
#include "V2.h"

/**
 * Place with some info used for finding.
 */
class FinderPlace {
    private:
        Dir::eDir m_startDir;
        V2 m_loc;
    public:
        FinderPlace(Dir::eDir startDir, const V2 &loc)
            : m_startDir(startDir), m_loc(loc) {}
        V2 getLoc() const { return m_loc; }
        Dir::eDir getStartDir() const { return m_startDir; }
};

#endif
