#ifndef HEADER_BASELISTENER_H
#define HEADER_BASELISTENER_H

class BaseMsg;
class SimpleMsg;
class IntMsg;
class StringMsg;

#include "INamed.h"

#include <string>

/**
 * Listen messages.
 */
class BaseListener : public INamed {
    protected:
        void registerWatcher(const std::string &param);
    public:
        virtual ~BaseListener() {}
        virtual void receiveSimple(const SimpleMsg *msg);
        virtual void receiveInt(const IntMsg *msg);
        virtual void receiveString(const StringMsg *msg);
};

#endif
