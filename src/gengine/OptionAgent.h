#ifndef HEADER_OPTIONAGENT_H
#define HEADER_OPTIONAGENT_H

class Environ;
class OptionParams;

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
        void prepareVersion();
        void prepareDataPaths();
        void prepareLang();
        void parseDashOpt(const std::string &arg,
                const OptionParams &params);
        void parseParamOpt(const std::string &arg,
                const OptionParams &params);
        bool splitOpt(const std::string &option,
                std::string *out_name, std::string *out_value);
        std::string getHelpInfo(const OptionParams &params) const;
        std::string getVersionInfo() const;
        void readSystemConfig();
        void readUserConfig();
    protected:
        virtual void own_init();
        virtual void own_shutdown();
    public:
        void parseCmdOpt(int argc, char *argv[],
                const OptionParams &params);
        void setParam(const std::string &name, const std::string &value);
        void setParam(const std::string &name, long value);
        void setPersistent(const std::string &name, const std::string &value);
        void setPersistent(const std::string &name, long value);
        void setDefault(const std::string &name, const std::string &value);
        void setDefault(const std::string &name, int value);

        std::string getParam(const std::string &name,
                const std::string &implicit="") const;
        int getAsInt(const std::string &name,
                int implicit=0) const;
        bool getAsBool(const std::string &name,
                bool implicit=false) const;

        void addWatcher(const std::string &name, BaseMsg *msg);
        void removeWatchers(const std::string &listenerName);
        void receiveString(const StringMsg *msg);
};

#endif
