#ifndef HEADER_ACTOR_H
#define HEADER_ACTOR_H

#include "NoCopy.h"

#include <string>

/**
 * Dialog actor.
 */
class Actor : public NoCopy {
    private:
        int m_index;
        bool m_busy;
    public:
        Actor();
        void setIndex(int model_index) { m_index = model_index; }
        bool equals(const Actor *other) const;

        bool isBusy() const { return m_busy; }
        void setBusy(bool busy) { m_busy = busy; }

        virtual std::string toString() const;
};

#endif
