#ifndef HEADER_BASEMSG_H
#define HEADER_BASEMSG_H

class BaseListener;

#include <string>

/**
 * Messgage.
 */
class BaseMsg {
    protected:
        std::string m_listenerName;
        std::string m_name;
    public:
        BaseMsg(const std::string &listenerName, const std::string &name);
        virtual ~BaseMsg() {}
        virtual BaseMsg *clone() const = 0;
        virtual void sendActual(BaseListener *listener) const = 0;

        void sendClone() const;
        bool equalsName(const std::string &name) const
        { return m_name == name; }
        const std::string &getMsgName() const { return m_name; }
        const std::string &getListenerName() const { return m_listenerName; }

        virtual std::string toString() const;
};

/**
 * Allow clone typed message.
 */
#define CLONE(TYPE) \
virtual TYPE *clone() const { return new TYPE(*this); }

#endif
