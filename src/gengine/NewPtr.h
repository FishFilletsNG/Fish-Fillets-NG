#ifndef HEADER_NEWPTR_H
#define HEADER_NEWPTR_H

class Object;

#include "NoCopy.h"

/**
 * Pointer to newly created object.
 * It will delete the object if it is not say other way.
 */
class NewPtr : public NoCopy {
    private:
        Object *m_newObject;
    public:
        explicit NewPtr(Object *new_object);
        virtual ~NewPtr();
        void ok() { m_newObject = 0; }
};

#endif
