#ifndef HEADER_CUBE_H
#define HEADER_CUBE_H

class Shape;
class Driver;
class Rules;
class View;
class Anim;

#include "V2.h"
#include "NoCopy.h"

#include <vector>

/**
 * A object in game.
 */
class Cube : public NoCopy {
    public:
        typedef std::vector<Cube*> t_models;
        enum eWeight {
            LIGHT,
            HEAVY,
            FIXED
        };
    private:
        V2 m_loc;
        bool m_alive;
        eWeight m_weight;
        eWeight m_power;
        bool m_lookLeft;
        int m_index;

        Shape *m_shape;
        Driver *m_driver;
        View *m_view;
        Rules *m_rules;
    public:
        Cube(const V2 &location,
                eWeight weight, eWeight power, bool alive,
                Driver *driver, View *view, Shape *shape);
        ~Cube();

        bool drive();
        void finishRound();

        void change_die();
        void change_turnSide();
        void change_setLocation(V2 loc) { m_loc = loc; }

        V2 getLocation() const { return m_loc; }
        bool isAlive() const { return m_alive; }
        const Shape *getShape() const { return m_shape; }

        bool isLookLeft() const { return m_lookLeft; }
        Anim *anim();
        Rules *rules() { return m_rules; }
        const Rules *const_rules() const { return m_rules; };

        void setIndex(int model_index) { m_index = model_index; }
        eWeight getWeight() const { return m_weight; }
        eWeight getPower() const { return m_power; }

        std::string toString() const;
};

#endif
