#ifndef HEADER_ONSTACK_H
#define HEADER_ONSTACK_H

#include "OnCondition.h"
#include "Cube.h"
#include "Rules.h"

/**
 * Test whether object can carry moved stack.
 */
class OnStack : public OnCondition {
    public:
        virtual bool isSatisfy(Cube *model) const
        {
            bool result = false;
            if (!model->isAlive()) {
                if (model->rules()->getDir() == Dir::DIR_NO
                        && model->rules()->isOnStrongPad(Cube::LIGHT))
                {
                        result = true;
                }
            }
            return result;
        }

        virtual bool isWrong(Cube *model) const
        {
            return model->isAlive();
        }
};

#endif
