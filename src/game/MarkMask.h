#ifndef HEADER_MARKMASK_H
#define HEADER_MARKMASK_H

class Field;

#include "NoCopy.h"
#include "Rules.h"
#include "Cube.h"

class MarkMask : public NoCopy {
    private:
        Cube *m_model;
        Field *m_field;
    private:
        void writeModel(Cube *model);
    public:
        MarkMask(Cube *model, Field *field);

        Cube::t_models getResist(Rules::eDir dir) const;
        void mask();
        void unmask();

};

#endif
