#ifndef HEADER_ROOMACCESS_H
#define HEADER_ROOMACCESS_H

class Room;

#include "NoCopy.h"

/**
 * Interface to access changing room.
 */
class RoomAccess : public NoCopy {
    private:
        Room *m_room;
    private:
        void checkRoom() const;
    public:
        RoomAccess();
        ~RoomAccess();
        void takeRoom(Room *new_room);

        void cleanRoom();
        bool isRoom() const { return !!m_room; }
        Room *room();
        const Room *const_room() const;
};

#endif
