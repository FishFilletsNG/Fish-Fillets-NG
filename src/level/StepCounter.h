#ifndef HEADER_STEPCOUNTER_H
#define HEADER_STEPCOUNTER_H

#include <string>

/**
 * Interface to number of steps.
 */
class StepCounter {
    public:
        virtual ~StepCounter() {}
        virtual int getStepCount() const = 0;
        virtual std::string getMoves() const = 0;
        virtual bool isPowerful() const = 0;
        virtual bool isDangerousMove() const = 0;
};

#endif
