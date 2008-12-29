#ifndef HEADER_OPTIONPARAMS_H
#define HEADER_OPTIONPARAMS_H

class Environ;

#include "NoCopy.h"

#include <map>
#include <string>

/**
 * Describe command line params.
 */
class OptionParams : public NoCopy {
    public:
        enum eType {
            TYPE_NUMBER,
            TYPE_BOOLEAN,
            TYPE_STRING,
            TYPE_PATH
        };
    private:
        struct Param {
            eType type;
            std::string help;
            Param(eType aType, const std::string &aHelp)
                : type(aType), help(aHelp) {}
        };
        typedef std::map<std::string,Param> t_params;
        t_params m_params;
        int m_maxSize;
    private:
        std::string getType(eType type) const;
    public:
        OptionParams() { m_maxSize = 0; }
        void addParam(const std::string &name, eType type,
                const std::string &help);

        std::string getHelp(const Environ *options) const;
        std::string getConfig(const Environ *options) const;
        void checkValidity(const std::string &name, const std::string &value) const;
};



#endif
