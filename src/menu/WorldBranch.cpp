/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldBranch.h"

#include "def-script.h"
#include "Log.h"
#include "Path.h"
#include "LevelNode.h"
#include "LevelStatus.h"
#include "ScriptState.h"
#include "LogicException.h"

//-----------------------------------------------------------------
    inline WorldBranch *
getBranch(lua_State *L)
{
    return static_cast<WorldBranch*>(script_getLeader(L));
}
//-----------------------------------------------------------------
/**
 * void branch_addNode(parent, codename, datafile, x, y,
 *          hidden=false, poster="")
 */
    int
script_branch_addNode(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *parent = luaL_checkstring(L, 1);
    const char *codename = luaL_checkstring(L, 2);
    const char *datafile = luaL_checkstring(L, 3);
    int nodeX = luaL_checkint(L, 4);
    int nodeY = luaL_checkint(L, 5);
    bool hidden = lua_toboolean(L, 6);
    const char *poster = luaL_optstring(L, 7, "");

    LevelNode *node = new LevelNode(codename,
                Path::dataReadPath(datafile), V2(nodeX, nodeY), poster);
    getBranch(L)->addNode(parent, node, hidden);
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
WorldBranch::WorldBranch(LevelNode *root)
{
    m_root = root;
}
//-----------------------------------------------------------------
/**
 * Execute script which will add nodes.
 * @return root node (can be NULL)
 */
LevelNode *
WorldBranch::parseMap(const Path &datafile)
{
    ScriptState script;
    script.registerLeader(this);
    script.registerFunc("branch_addNode", script_branch_addNode);
    script.doFile(datafile);

    if (m_root && m_root->getState() < LevelNode::STATE_OPEN) {
        m_root->setState(LevelNode::STATE_OPEN);
    }
    return m_root;
}
//-----------------------------------------------------------------
/**
 * Add new node to branch.
 */
void
WorldBranch::addNode(const std::string &parent, LevelNode *new_node,
        bool hidden)
{
    prepareNode(new_node, hidden);
    insertNode(parent, new_node);
}
//-----------------------------------------------------------------
/**
 * Returns true when level will solved in the past.
 */
bool
WorldBranch::wasSolved(const std::string &codename)
{
    Path solved =
        Path::dataReadPath(LevelStatus::getSolutionFilename(codename));
    return solved.exists();
}
//-----------------------------------------------------------------
/**
 * Set node state.
 * @param hidden whether node is start node of hidden branch
 */
    void
WorldBranch::prepareNode(LevelNode *node, bool hidden)
{
    if (wasSolved(node->getCodename())) {
        node->setState(LevelNode::STATE_SOLVED);
    }
    else if (hidden) {
        node->setState(LevelNode::STATE_HIDDEN);
    }
    else {
        node->setState(LevelNode::STATE_FAR);
    }
}
//-----------------------------------------------------------------
/**
 * Insert node as parent child.
 * @throws LogicException when error occurs
 */
void
WorldBranch::insertNode(const std::string &parent, LevelNode *new_node)
{
    try {
        if (parent == "" && m_root) {
            throw LogicException(ExInfo("there is a one root node already")
                    .addInfo("root", m_root->getCodename())
                    .addInfo("new_node", new_node->getCodename()));
        }

        if (m_root) {
            LevelNode *parentNode = m_root->findNamed(parent);
            if (parentNode) {
                parentNode->addChild(new_node);
            }
            else {
                throw LogicException(ExInfo("there is no such parent node")
                        .addInfo("parent", parent)
                        .addInfo("new_node", new_node->getCodename()));
            }
        }
        else {
            if (parent != "") {
                LOG_WARNING(ExInfo("root node should have empty parent")
                        .addInfo("parent", parent)
                        .addInfo("new_node", new_node->getCodename()));
            }
            m_root = new_node;
        }
    }
    catch (...) {
        delete new_node;
        throw;
    }
}

