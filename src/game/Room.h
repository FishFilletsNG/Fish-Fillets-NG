#ifndef HEADER_ROOM_H
#define HEADER_ROOM_H

class Field;
class Picture;

#include "NoCopy.h"
#include "Path.h"
#include "Field.h"
#include "Cube.h"

/**
 * Room with level.
 */
class Room : public NoCopy {
    private:
        Picture *m_bg;
        Field *m_field;
        Cube::t_models m_models;
    private:
        void prepareRound();
        bool falldown();
        void driving();
        bool finishRound();
    public:
        Room(int w, int h, const Path &picture);
        ~Room();

        int addModel(Cube *model);
        Cube *getModel(int model_index);
        bool nextRound();

        int getW() const { return m_field->getW(); }
        int getH() const { return m_field->getH(); }

};

#endif
