#ifndef HEADER_OPTIONAGENT_H
#define HEADER_OPTIONAGENT_H

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
        typedef std::map<std::string,std::string> t_values;
        typedef std::multimap<std::string,BaseMsg*> t_watchers;
        t_values m_values;
        t_watchers m_watchers;
    private:
        void prepareDataPaths();
        bool splitOpt(const std::string &option,
                std::string *out_name, std::string *out_value);
    protected:
        virtual void own_init();
        virtual void own_shutdown();
    public:
        virtual ~OptionAgent();

        void parseCmdOpt(int argc, char *argc[]);
        void setParam(const std::string &name, const std::string &value);
        void setParam(const std::string &name, long value);

        std::string getParam(const std::string &name,
                const std::string &implicit="");
        int getAsInt(const std::string &name,
                int implicit=0);

        void addWatcher(const std::string &name, BaseMsg *msg);
};

#endif
