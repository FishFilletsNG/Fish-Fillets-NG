#ifndef HEADER_MODELFACTORY_H
#define HEADER_MODELFACTORY_H

class Unit;

#include "V2.h"
#include "Cube.h"

#include <string>

class ModelFactory {
    private:
        static void createParams(const std::string &kind,
                Cube::eWeight *out_weight, Cube::eWeight *out_power,
                bool *out_alive);
    public:
        static Cube *createModel(const std::string &kind, const V2 &loc,
                const std::string &shape);
        static Unit *createUnit(const std::string &kind);
        static Cube *createBorder();
};

#endif
