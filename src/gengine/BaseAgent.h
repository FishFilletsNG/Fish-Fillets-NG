#ifndef HEADER_BASEAGENT_H
#define HEADER_BASEAGENT_H

#include "NoCopy.h"
#include "BaseListener.h"
#include "ExInfo.h"
#include "NameException.h"
#include "AgentPack.h"

#include <string>

/**
 * Ancestor for all agents.
 */
class BaseAgent : public NoCopy, public BaseListener {
    private:
        bool m_initialized;
    protected:
        virtual void own_init() {}
        virtual void own_update() {}
        virtual void own_shutdown() {}
    public:
        BaseAgent();
        virtual ~BaseAgent() {}
        bool isInitialized() { return m_initialized; }

        void init();
        void update();
        void shutdown();
};

/**
 * Example: AGENT(VideoAgent, Name::VIDEO_NAME)
 *
 * Enables to obtain typed pointer
 * static VideoAgent *VideoAgent::agent();
 */
#define AGENT(TYPE, NAME) \
public: \
virtual const std::string &getName() const { return (NAME); } \
static TYPE * \
TYPE::agent() \
{ \
    TYPE *result = dynamic_cast<TYPE *>(AgentPack::getAgent(sName())); \
    if (NULL == result) { \
        throw NameException(ExInfo("cannot cast agent") \
                .addInfo("name", sName())); \
    } \
    return result; \
} \
private: \
static const std::string &sName() { return (NAME); }

#endif
