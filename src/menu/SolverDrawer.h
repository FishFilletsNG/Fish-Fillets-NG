#ifndef HEADER_SOLVERDRAWER_H
#define HEADER_SOLVERDRAWER_H

class LevelStatus;

#include "VBox.h"

/**
 * Draws the best solver.
 */
class SolverDrawer : public VBox {
    public:
        SolverDrawer(LevelStatus *status);
};

#endif
