#ifndef HEADER_FINDERFIELD_H
#define HEADER_FINDERFIELD_H

class V2;

#include "NoCopy.h"

/**
 * Array of closed nodes used for finding.
 */
class FinderField : public NoCopy {
    private:
        bool **m_closed;
        int m_w;
        int m_h;
    public:
        FinderField(int w, int h);
        virtual ~FinderField();
        void reset();

        void markClosed(const V2 &loc);
        bool isClosed(const V2 &loc) const;
};

#endif
