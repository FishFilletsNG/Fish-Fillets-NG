#ifndef HEADER_OPTIONAGENT_H
#define HEADER_OPTIONAGENT_H

class Environ;

#include "BaseAgent.h"
#include "Name.h"

#include <string>
#include <map>

/**
 * Game options.
 */
class OptionAgent : public BaseAgent {
    AGENT(OptionAgent, Name::OPTION_NAME);
    private:
        static const char *CONFIG_FILE;
        Environ *m_environ;
    private:
        void prepareDataPaths();
        void prepareLang();
        bool splitOpt(const std::string &option,
                std::string *out_name, std::string *out_value);
    protected:
        virtual void own_init();
        virtual void own_shutdown();
    public:
        void parseCmdOpt(int argc, char *argv[]);
        void setParam(const std::string &name, const std::string &value);
        void setParam(const std::string &name, long value);
        void setPersistent(const std::string &name, const std::string &value);
        void setPersistent(const std::string &name, long value);

        std::string getParam(const std::string &name,
                const std::string &implicit="");
        int getAsInt(const std::string &name,
                int implicit=0);

        void addWatcher(const std::string &name, BaseMsg *msg);
};

#endif
