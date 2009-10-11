#ifndef HEADER_RULES_H
#define HEADER_RULES_H

class V2;
class MarkMask;
class Field;
class OnCondition;

#include "NoCopy.h"
#include "Cube.h"
#include "Dir.h"

/**
 * Game rules.
 */
class Rules : public NoCopy {
    private:
        Dir::eDir m_dir;
        bool m_readyToDie;
        bool m_readyToTurn;
        bool m_readyToActive;
        bool m_pushing;
        bool m_lastFall;
        int m_outDepth;
        Dir::eDir m_touchDir;

        Cube *m_model;
        MarkMask *m_mask;
    private:
        bool checkDeadMove();
        bool checkDeadFall();
        bool checkDeadStress();

        bool isOnStack();
        bool isOnCond(const OnCondition &cond);
        bool isOnWall();

        bool isFalling() const;
        Cube::t_models whoIsFalling();

        bool isHeavier(Cube::eWeight power) const;
        Cube::t_models whoIsHeavier(Cube::eWeight power);

        bool canDir(Dir::eDir dir, Cube::eWeight power);
        bool touchSpec(Dir::eDir dir);
        void setTouched(Dir::eDir dir);
        void moveDirBrute(Dir::eDir dir);

        void freeOldPos();
    public:
        Rules(Cube *model);
        ~Rules();
        void takeField(Field *field);
        void change_setLocation(const V2 &loc);

        void occupyNewPos();
        bool checkDead(Cube::eAction lastAction);
        void changeState();
        void finishRound();

        int actionOut();
        void actionFall();
        bool clearLastFall();
        bool actionMoveDir(Dir::eDir dir);
        void actionTurnSide() { m_readyToTurn = true; }
        void actionActivate() { m_readyToActive = true; }

        Dir::eDir getDir() const { return m_dir; }
        Dir::eDir getTouchDir() const { return m_touchDir; }
        std::string getAction() const;
        std::string getState() const;
        bool isOnStrongPad(Cube::eWeight weight);
        bool isAtBorder() const;
        bool isFreePlace(const V2 &loc) const;
        const Cube::t_models getResist(Dir::eDir dir) const;
        bool isPushing() const { return m_pushing; };
        void resetLastDir() { m_dir = Dir::DIR_NO; }
        bool canMoveOthers(Dir::eDir dir, Cube::eWeight weight);
};

#endif
