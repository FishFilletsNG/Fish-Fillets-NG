#ifndef HEADER_CUBE_H
#define HEADER_CUBE_H

class Shape;
class View;
class Driver;
class MarkMask;
class Field;

#include "V2.h"
#include "NoCopy.h"

#include <vector>

class Cube : public NoCopy {
    public:
        typedef std::vector<Cube*> t_models;
        enum eWeight {
            LIGHT,
            HEAVY,
            FIXED
        };
        enum eDir {
            DIR_NO,
            DIR_UP,
            DIR_DOWN,
            DIR_LEFT,
            DIR_RIGHT
        };
    private:
        V2 m_loc;
        bool m_alive;
        bool m_readyToDie;
        eWeight m_weight;
        eWeight m_power;
        eDir m_dir;

        Shape *m_shape;
        Driver *m_driver;
        View *m_view;
        MarkMask *m_mask;
    private:
        bool checkDeadMove();
        bool checkDeadFall();
        bool checkDeadStress(eWeight power);

        bool isOnStack();
        bool isOnWall();
        bool isOnStrong(eWeight weight);
        bool isFallingOnFish();
        bool isHeavier(eWeight power);

        bool canMoveOthers(eDir dir, eWeight weight);
        void moveDirBrute(eDir dir);
    public:
        Cube(const V2 &location,
                eWeight weight, eWeight power, bool alive,
                Driver *driver, View *view, Shape *shape);
        ~Cube();
        void takeField(Field *field);

        void occupyNewPos();
        void checkDead();
        void applyDead();
        bool fall();
        bool drive();
        void finishRound();

        bool canFall();
        bool canDir(eDir dir, eWeight power);
        bool moveDir(eDir dir);

        V2 getLocation() const { return m_loc; }
        bool isAlive() const { return m_alive; }
        bool isWall() const { return m_weight >= FIXED; }
        const Shape *getShape() const { return m_shape; }
        View *view() { return m_view; }

        eWeight getWeight() const { return m_weight; }
        eWeight getPower() const { return m_power; }
        eDir getDir() const { return m_dir; }

        void dir2xy(eDir dir, int *out_x, int *out_y) const;
        std::string toString() const;
};

#endif
