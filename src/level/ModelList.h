#ifndef HEADER_MODELLIST_H
#define HEADER_MODELLIST_H

class View;
class Landslip;

#include "Cube.h"

#include <vector>

/**
 * Wrapper around list of models.
 */
class ModelList {
    private:
        const Cube::t_models *m_models;
    public:
        ModelList(const Cube::t_models *models);
        int size() const { return m_models->size(); }

        void drawOn(View *view) const;
        bool stoneOn(Landslip *slip) const;
        bool fallOn(Landslip *slip) const;
};

#endif
