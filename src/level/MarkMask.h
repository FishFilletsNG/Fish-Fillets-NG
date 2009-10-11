#ifndef HEADER_MARKMASK_H
#define HEADER_MARKMASK_H

class V2;
class Field;

#include "NoCopy.h"
#include "Dir.h"
#include "Cube.h"

/**
 * Marks and unmasks object from game field.
 */
class MarkMask : public NoCopy {
    private:
        Cube *m_model;
        Field *m_field;
    private:
        void writeModel(Cube *model, Cube *toOverride);
        bool canGo(Dir::eDir dir) const;
        bool isInRoom() const;
    public:
        MarkMask(Cube *model, Field *field);

        Cube::t_models getResist(Dir::eDir dir) const;
        Cube::t_models getPlacedResist(const V2 &loc) const;
        void mask();
        void unmask();

        Dir::eDir getBorderDir() const;
        bool isFullyOut() const;
};

#endif
