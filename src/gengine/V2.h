#ifndef HEADER_V2_H
#define HEADER_V2_H

#include "StringTool.h"

#include <string>

/**
 * Vector x,y.
 */
class V2 {
    private:
        int m_x;
        int m_y;
    public:
        V2(int x, int y) { m_x = x; m_y = y; }

        int getX() const { return m_x; }
        int getY() const { return m_y; }

        V2 composition(const V2 &other) const
        {
            return V2(m_x + other.m_x, m_y + other.m_y);
        }

        std::string toString() const
        {
            return "[" + StringTool::toString(m_x)
                + "," + StringTool::toString(m_y) + "]";
        }
};


#endif
