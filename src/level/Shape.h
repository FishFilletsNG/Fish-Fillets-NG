#ifndef HEADER_SHAPE_H
#define HEADER_SHAPE_H

#include "NoCopy.h"
#include "V2.h"

#include <string>
#include <vector>

/**
 * Stores model shape.
 * It is uses by MarkMask to ask Field under shape.
 */
class Shape : public NoCopy {
    public:
        typedef std::vector<V2> t_marks;
        typedef t_marks::const_iterator const_iterator;
    private:
        t_marks m_marks;
        int m_w;
        int m_h;
    public:
        Shape(const std::string &shape);

        const_iterator marksBegin() const { return m_marks.begin(); }
        const_iterator marksEnd() const { return m_marks.end(); }
        int getW() const { return m_w; }
        int getH() const { return m_h; }

        std::string toString() const;
};

#endif
