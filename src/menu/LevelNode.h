#ifndef HEADER_LEVELNODE_H
#define HEADER_LEVELNODE_H

class Level;
class NodeDrawer;

#include "V2.h"
#include "Path.h"
#include "NoCopy.h"

#include <string>
#include <vector>

/**
 * Node on the map.
 */
class LevelNode : public NoCopy {
    public:
        enum eState {
            STATE_HIDDEN,
            STATE_FAR,
            STATE_OPEN,
            STATE_SOLVED
        };
    private:
        static const int DOT_RADIUS = 13;
        std::string m_codename;
        Path m_datafile;
        V2 m_loc;
        eState m_state;
        int m_depth;
        typedef std::vector<class LevelNode*> t_children;
        t_children m_children;
    private:
        bool isUnder(const V2 &cursor) const;
    public:
        LevelNode(const std::string &codename, const Path &datafile,
                const V2 &loc, int depth);
        virtual ~LevelNode();
        int getDepth() const { return m_depth; }

        std::string getCodeName() const { return m_codename; }
        void setState(eState state);
        V2 getLoc() const { return m_loc; }
        eState getState() const { return m_state; }
        Level *createLevel() const;

        void addChild(LevelNode *node);

        LevelNode *findSelected(const V2 &cursor);
        void drawPath(const NodeDrawer *drawer) const;
};

#endif
