#ifndef HEADER_MESSAGERAGENT_H
#define HEADER_MESSAGERAGENT_H


#include "BaseAgent.h"
#include "Name.h"

#include <map>


/**
 * List of listeners.
 */
class MessagerAgent : public BaseAgent {
    AGENT(MessagerAgent, Name::MESSAGER_NAME);
    private:
        typedef std::map<std::string,BaseListener*> t_listeners;
        t_listeners m_listeners;
    public:
        void addListener(BaseListener *listener);
        void removeListener(const std::string &name);

        void forwardNewMsg(BaseMsg *msg);
};

#endif
