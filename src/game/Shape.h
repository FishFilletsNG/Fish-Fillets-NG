#ifndef HEADER_SHAPE_H
#define HEADER_SHAPE_H

#include "NoCopy.h"
#include "V2.h"

#include <string>
#include <vector>

class Shape : public NoCopy {
    private:
        typedef std::vector<V2> t_marks;
        t_marks m_marks;
    public:
        typedef t_marks::const_iterator const_iterator;
    public:
        Shape(const std::string &shape);

        const_iterator begin() const { return m_marks.begin(); }
        const_iterator end() const { return m_marks.end(); }

        std::string toString() const;
};

#endif
