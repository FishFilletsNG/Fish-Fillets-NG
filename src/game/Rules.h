#ifndef HEADER_RULES_H
#define HEADER_RULES_H

class MarkMask;
class Field;

#include "NoCopy.h"
#include "Cube.h"

/**
 * Game rules.
 */
class Rules : public NoCopy {
    public:
        enum eDir {
            DIR_NO,
            DIR_UP,
            DIR_DOWN,
            DIR_LEFT,
            DIR_RIGHT
        };
    private:
        eDir m_dir;
        bool m_readyToDie;
        bool m_readyToTurn;

        Cube *m_model;
        MarkMask *m_mask;
    private:
        bool checkDeadMove();
        bool checkDeadFall();
        bool checkDeadStress();

        bool isWall() const;
        bool isOnStack();
        bool isOnWall();
        bool isOnStrongFish(Cube::eWeight weight);

        bool isFalling() const;
        Cube::t_models whoIsFalling();

        bool isHeavier(Cube::eWeight power) const;
        Cube::t_models whoIsHeavier(Cube::eWeight power);

        bool canMoveOthers(eDir dir, Cube::eWeight weight);
        void moveDirBrute(eDir dir);
    public:
        Rules(Cube *model);
        ~Rules();
        void takeField(Field *field);

        void occupyNewPos();
        void checkDead();
        void prepareRound();
        void freeOldPos();

        bool actionFall();
        bool actionMoveDir(eDir dir);
        void actionTurnSide();

        bool canFall();
        bool canDir(eDir dir, Cube::eWeight power);

        eDir getDir() const { return m_dir; }
        std::string getAction() const;

        V2 dir2xy(eDir dir) const;
};

#endif
