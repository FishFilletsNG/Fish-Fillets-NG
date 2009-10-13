#ifndef HEADER_CUBE_H
#define HEADER_CUBE_H

class Shape;
class Anim;
class Rules;
class DialogStack;

#include "V2.h"
#include "Goal.h"
#include "NoCopy.h"
#include "Dir.h"

#include <vector>

/**
 * A object in game.
 */
class Cube : public NoCopy {
    public:
        typedef std::vector<Cube*> t_models;
        enum eWeight {
            NONE,
            LIGHT,
            HEAVY,
            FIXED
        };
        enum eAction {
            ACTION_NO,
            ACTION_FALL,
            ACTION_MOVE
        };
    private:
        int m_index;
        bool m_busy;
        V2 m_loc;
        bool m_alive;
        bool m_out;
        eWeight m_weight;
        eWeight m_power;
        bool m_lookLeft;
        bool m_lost;

        Shape *m_shape;
        Anim *m_anim;
        Rules *m_rules;
        Goal m_goal;
        Dir::eDir m_outDir;
        int m_outCapacity;
        const DialogStack *m_dialogs;
    public:
        Cube(const V2 &location,
                eWeight weight, eWeight power, bool alive,
                Shape *shape);
        ~Cube();
        void setGoal(const Goal &goal) { m_goal = goal; }
        void setIndex(int model_index) { m_index = model_index; }
        int getIndex() const { return m_index; }
        bool isBusy() const { return m_busy; }
        void setBusy(bool busy) { m_busy = busy; }

        void change_die();
        void change_goingOut();
        void change_goOut();
        void change_remove();
        void change_turnSide();
        void change_setLocation(const V2 &loc) { m_loc = loc; }

        V2 getLocation() const { return m_loc; }
        bool isAlive() const { return m_alive; }
        bool isLeft() const { return m_lookLeft; }
        bool isOut() const { return m_out; }
        bool isLost() const { return m_lost; }
        bool isSatisfy() const { return m_goal.isSatisfy(this); }
        bool isWrong() const { return m_goal.isWrong(this); }
        bool isWall() const { return m_weight >= Cube::FIXED; }
        bool shouldGoOut() const { return m_goal.shouldGoOut(); }
        bool isBorder() const { return m_index == -1; }

        eWeight getWeight() const { return m_weight; }
        eWeight getPower() const { return m_power; }
        const Shape *shape() const { return m_shape; }
        Dir::eDir getLastMoveDir() const;

        bool isOutDir(Dir::eDir dir) const { return m_outDir == dir; }
        Dir::eDir getOutDir() { return m_outDir; }
        int getOutCapacity() { return m_outCapacity; }
        void setOutDir(Dir::eDir outDir,
                int capacity=2, eWeight weight=Cube::FIXED);
        void decOutCapacity();
        void setExtraParams();

        bool isTalking() const;
        void takeDialogs(const DialogStack *dialogs) { m_dialogs = dialogs; }

        bool isDisintegrated();
        bool isInvisible();
        Anim *anim() { return m_anim; }
        const Anim *const_anim() const { return m_anim; }
        Rules *rules() { return m_rules; }
        const Rules *const_rules() const { return m_rules; }

        virtual std::string toString() const;
};

#endif
