#ifndef HEADER_INAMED_H
#define HEADER_INAMED_H

#include <string>

/**
 * Interface - named object.
 */
class INamed {
    public:
        virtual ~INamed() {}
        virtual const std::string &getName() const = 0;
};

#endif
