#ifndef HEADER_MODELFACTORY_H
#define HEADER_MODELFACTORY_H

class V2;
class Unit;

#include "Cube.h"
#include "ControlSym.h"
#include "Dir.h"

#include <string>

/**
 * Knows how to create new models.
 */
class ModelFactory {
    private:
        static void createParams(const std::string &kind,
                Cube::eWeight *out_weight, Cube::eWeight *out_power,
                bool *out_alive);
        static ControlSym parseExtraControlSym(const std::string &kind);
        static Cube *createOutputItem(const std::string &kind, const V2 &loc,
                const std::string &shape);
    public:
        static Cube *createModel(const std::string &kind, const V2 &loc,
                const std::string &shape);
        static Unit *createUnit(const std::string &kind);
        static Cube *createBorder();
};

#endif
