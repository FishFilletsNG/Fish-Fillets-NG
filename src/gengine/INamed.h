#ifndef HEADER_INAMED_H
#define HEADER_INAMED_H

/**
 * Interface - named object.
 */
class INamed {
    public:
        virtual ~INamed() {}
        virtual const char *getName() const = 0;
};

#endif
