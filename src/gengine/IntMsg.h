#ifndef HEADER_INTMSG_H
#define HEADER_INTMSG_H

#include "BaseMsg.h"
#include "BaseListener.h"

/**
 * Message with integer value.
 */
class IntMsg : public BaseMsg {
    private:
        int m_value;
    public:
        CLONE(IntMsg);
        IntMsg(BaseListener *listener, const std::string &name,
                int value)
            : BaseMsg(listener->getName(), name), m_value(value) {}
        IntMsg(const std::string &listenerName, const std::string &name,
                int value)
            : BaseMsg(listenerName, name), m_value(value) {}

        virtual void sendActual(BaseListener *listener) const
        {
            listener->receiveInt(this);
        }
        int getValue() const { return m_value; }
        virtual std::string toString() const;
};


#endif
