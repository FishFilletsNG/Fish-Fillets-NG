#ifndef HEADER_ONSTRONGPAD_H
#define HEADER_ONSTRONGPAD_H

#include "OnCondition.h"
#include "Cube.h"

/**
 * Test whether model is on Wall or on powerful fish.
 */
class OnStrongPad : public OnCondition {
    private:
        Cube::eWeight m_weight;
    public:
        OnStrongPad(Cube::eWeight weight) { m_weight = weight; }

        virtual bool isSatisfy(Cube *model) const
        {
            return model->isWall()
                || (model->isAlive() && model->getPower() >= m_weight);
        }

        virtual bool isWrong(Cube *model) const
        {
            return model->isAlive() && model->getPower() < m_weight;
        }
};

#endif
