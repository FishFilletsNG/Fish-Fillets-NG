#ifndef HEADER_TIMERAGENT_H
#define HEADER_TIMERAGENT_H

#include "BaseAgent.h"
#include "Name.h"

#include "SDL.h"

/**
 * Delay and framerame.
 */
class TimerAgent : public BaseAgent {
    AGENT(TimerAgent, Name::TIMER_NAME);
    private:
        static const int TIMEINTERVAL = 50;

        Uint32 m_lastTime;
        Uint32 m_nextTime;
        Uint32 m_deltaTime;
    protected:
        virtual void own_init();
        virtual void own_update();
    public:
        Uint32 getDeltaTime() const { return m_deltaTime; }
};

#endif
