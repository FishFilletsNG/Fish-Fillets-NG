#ifndef HEADER_CUBE_H
#define HEADER_CUBE_H

class Shape;
class Rules;
class View;
class Anim;

#include "V2.h"
#include "Actor.h"
#include "Goal.h"

#include <vector>

/**
 * A object in game.
 */
class Cube : public Actor {
    public:
        typedef std::vector<Cube*> t_models;
        enum eWeight {
            NONE,
            LIGHT,
            HEAVY,
            FIXED
        };
    private:
        V2 m_loc;
        bool m_alive;
        bool m_out;
        eWeight m_weight;
        eWeight m_power;
        bool m_lookLeft;

        Shape *m_shape;
        View *m_view;
        Rules *m_rules;
        Goal m_goal;
    public:
        static Cube *createBorder();
        Cube(const V2 &location,
                eWeight weight, eWeight power, bool alive,
                View *view, Shape *shape);
        ~Cube();
        void setGoal(const Goal &goal) { m_goal = goal; }

        void finishRound();

        void change_die();
        void change_goOut();
        void change_turnSide();
        void change_setLocation(V2 loc) { m_loc = loc; }

        V2 getLocation() const { return m_loc; }
        bool isAlive() const { return m_alive; }
        bool isLeft() const { return m_lookLeft; }
        bool isOut() const { return m_out; }
        bool isSatisfy() const { return m_goal.isSatisfy(this); }
        bool shouldGoOut() const { return m_goal.shouldGoOut(); }

        eWeight getWeight() const { return m_weight; }
        eWeight getPower() const { return m_power; }
        const Shape *shape() const { return m_shape; }

        Anim *anim();
        Rules *rules() { return m_rules; }
        const Rules *const_rules() const { return m_rules; };

        virtual std::string toString() const;
};

#endif
