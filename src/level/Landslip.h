#ifndef HEADER_LANDSLIP_H
#define HEADER_LANDSLIP_H

#include "NoCopy.h"
#include "Cube.h"
#include "ModelList.h"

/**
 * Landslip for every round.
 */
class Landslip : public NoCopy {
    private:
        ModelList m_models;
        Cube::eWeight m_impact;
        bool *m_stoned;
    private:
        bool isOnPad(const Cube *model) const;
        bool isFixed(const Cube *model) const;
        bool isStoned(const Cube *model) const;
        void stone(const Cube *model);
    public:
        Landslip(const ModelList &models);
        ~Landslip();

        bool computeFall();
        Cube::eWeight getImpact() { return m_impact; }

        bool stoneModel(const Cube *model);
        bool fallModel(Cube *model);
};

#endif
