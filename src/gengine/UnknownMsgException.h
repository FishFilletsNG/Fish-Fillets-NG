#ifndef HEADER_UNKNOWNMSGEXCEPTION_H
#define HEADER_UNKNOWNMSGEXCEPTION_H

#include "BaseException.h"

#include "BaseMsg.h"

/**
 * Exception about unknown msg.
 */
class UnknownMsgException : public BaseException  {
    public:
        UnknownMsgException(const BaseMsg *msg)
            : BaseException(ExInfo("unknown message")
                    .addInfo("msg", msg->toString()))
            {}
};

#endif
