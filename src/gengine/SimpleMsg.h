#ifndef HEADER_SIMPLEMSG_H
#define HEADER_SIMPLEMSG_H

#include "BaseMsg.h"
#include "BaseListener.h"

/**
 * Simple text messgage.
 */
class SimpleMsg : public BaseMsg {
    public:
        CLONE(SimpleMsg);
        SimpleMsg(BaseListener *listener, const std::string &name)
            : BaseMsg(listener->getName(), name) {}
        SimpleMsg(const std::string &listenerName, const std::string &name)
            : BaseMsg(listenerName, name) {}

        virtual void sendActual(BaseListener *listener) const
        {
            listener->receiveSimple(this);
        }
};


#endif
