#ifndef HEADER_ENVIRON_H
#define HEADER_ENVIRON_H

class Path;
class BaseMsg;

#include "NoCopy.h"

#include <string>
#include <map>

/**
 * Params.
 */
class Environ : public NoCopy {
    private:
        typedef std::map<std::string,std::string> t_values;
        typedef std::multimap<std::string,BaseMsg*> t_watchers;
        t_values m_values;
        t_watchers m_watchers;
    public:
        virtual ~Environ();
        void store(const Path &file);

        void setParam(const std::string &name, const std::string &value);
        void setParam(const std::string &name, long value);

        std::string getParam(const std::string &name,
                const std::string &implicit="");
        int getAsInt(const std::string &name,
                int implicit=0);

        void addWatcher(const std::string &name, BaseMsg *msg);
        std::string toString() const;
};

#endif
