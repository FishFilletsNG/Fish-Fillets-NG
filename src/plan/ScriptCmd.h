#ifndef HEADER_SCRIPTCMD_H
#define HEADER_SCRIPTCMD_H

class ScriptState;

#include "NoCopy.h"
#include "Command.h"

/**
 * Command which execute script function.
 */
class ScriptCmd : public Command, public NoCopy {
    private:
        int m_funcRef;
        ScriptState *m_script;
    public:
        ScriptCmd(ScriptState *script, int funcRef);
        virtual ~ScriptCmd();

        virtual bool finish(int count);
};

#endif
