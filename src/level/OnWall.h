#ifndef HEADER_ONWALL_H
#define HEADER_ONWALL_H

#include "OnCondition.h"
#include "Cube.h"

/**
 * Test whether model is on Wall.
 */
class OnWall : public OnCondition {
    public:
        virtual bool isSatisfy(Cube *model) const
        {
            return model->isWall();
        }

        virtual bool isWrong(Cube *model) const
        {
            return model->isAlive();
        }
};

#endif
