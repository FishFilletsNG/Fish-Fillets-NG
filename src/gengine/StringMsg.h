#ifndef HEADER_STRINGMSG_H
#define HEADER_STRINGMSG_H

#include "BaseMsg.h"
#include "BaseListener.h"

#include <string>

/**
 * Message with string value.
 */
class StringMsg : public BaseMsg {
    private:
        std::string m_value;
    public:
        CLONE(StringMsg);
        StringMsg(BaseListener *listener, const std::string &name,
                const std::string &value)
            : BaseMsg(listener->getName(), name), m_value(value) {}
        StringMsg(const std::string &listenerName, const std::string &name,
                const std::string &value)
            : BaseMsg(listenerName, name), m_value(value) {}

        virtual void sendActual(BaseListener *listener) const
        {
            listener->receiveString(this);
        }
        const std::string &getValue() const { return m_value; }
        virtual std::string toString() const;
};


#endif
