/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "CommandQueue.h"

#include "Command.h"

//-----------------------------------------------------------------
CommandQueue::CommandQueue()
{
    m_count = 0;
}
//-----------------------------------------------------------------
/**
 * Remove all commands.
 */
CommandQueue::~CommandQueue()
{
    removeAll();
}
//-----------------------------------------------------------------
/**
 * Add new command at the end of queue.
 */
void
CommandQueue::planCommand(Command *new_command)
{
    m_commands.push_back(new_command);
}
//-----------------------------------------------------------------
/**
 * Execute first command.
 * Execute none command when queue is empty.
 * If the command returns true, remove him from queue.
 * @return true when a command was executed
 */
bool
CommandQueue::executeFirst()
{
    bool result = false;
    if (!m_commands.empty()) {
        Command *command = m_commands.front();
        if (command->finish(m_count)) {
            m_commands.pop_front();
            m_count = 0;
            delete command;
        }
        else {
            m_count++;
        }
        result = true;
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Remove all commands.
 */
void
CommandQueue::removeAll()
{
    t_commands::iterator end = m_commands.end();
    for (t_commands::iterator i = m_commands.begin(); i != end; ++i) {
        delete (*i);
    }
    m_commands.clear();
    m_count = 0;
}

