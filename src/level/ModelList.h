#ifndef HEADER_MODELLIST_H
#define HEADER_MODELLIST_H

class Cube;
class View;

#include <vector>

/**
 * Wrapper around list of models.
 */
class ModelList {
    private:
        typedef std::vector<Cube*> t_models;
        const t_models *m_models;
    public:
        ModelList(const t_models *models);
        void drawOn(View *view) const;
};

#endif
