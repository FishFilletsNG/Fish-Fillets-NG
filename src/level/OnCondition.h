#ifndef HEADER_ONCONDITION_H
#define HEADER_ONCONDITION_H

class Cube;

/**
 * Test condition.
 */
class OnCondition {
    public:
        virtual ~OnCondition() {}

        virtual bool isSatisfy(Cube *model) const = 0;
        virtual bool isWrong(Cube *model) const = 0;
};

#endif
