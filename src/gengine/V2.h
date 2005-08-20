#ifndef HEADER_V2_H
#define HEADER_V2_H

#include "StringTool.h"

#include <string>
#include <assert.h>

/**
 * Vector x,y.
 * Constant object.
 */
class V2 {
    private:
        int m_x;
        int m_y;
    public:
        V2(int x, int y) { m_x = x; m_y = y; }

        inline int getX() const { return m_x; }
        inline int getY() const { return m_y; }

        V2 plus(const V2 &other) const
        {
            return V2(m_x + other.m_x, m_y + other.m_y);
        }
        V2 minus(const V2 &other) const
        {
            return V2(m_x - other.m_x, m_y - other.m_y);
        }
        V2 scale(int rate) const
        {
            return V2(m_x * rate, m_y * rate);
        }
        V2 shrink(int rate) const
        {
            assert(rate != 0);
            return V2(m_x / rate, m_y / rate);
        }
        bool equals(const V2 &other)
        {
            return m_x == other.m_x && m_y == other.m_y;
        }

        std::string toString() const
        {
            return "[" + StringTool::toString(m_x)
                + "," + StringTool::toString(m_y) + "]";
        }
};

#endif
