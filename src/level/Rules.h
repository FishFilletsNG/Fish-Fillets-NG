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
        enum eFall {
            FALL_NO,
            FALL_NOW,
            FALL_LAST
        };
    private:
        eDir m_dir;
        bool m_readyToDie;
        bool m_readyToTurn;
        bool m_readyToGoout;
        bool m_readyToActive;
        bool m_readyToRemove;
        bool m_pushing;
        bool m_lastFall;

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

        bool canFall();
        bool canDir(eDir dir, Cube::eWeight power);
        bool canMoveOthers(eDir dir, Cube::eWeight weight);
        void moveDirBrute(eDir dir);

        void freeOldPos();
    public:
        Rules(Cube *model);
        ~Rules();
        void takeField(Field *field);

        void occupyNewPos();
        bool checkDead();
        bool checkOut();
        void changeState();
        void finishRound();

        eFall actionFall();
        bool actionMoveDir(eDir dir);
        void actionTurnSide() { m_readyToTurn = true; }
        void actionActivate() { m_readyToActive = true; }

        eDir getDir() const { return m_dir; }
        std::string getAction() const;
        std::string getState() const;

        static V2 dir2xy(eDir dir);
};

#endif
