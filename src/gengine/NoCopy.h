#ifndef HEADER_NOCOPY_H
#define HEADER_NOCOPY_H

/**
 * Class with private copy constructor and assign operator.
 */
class NoCopy {
    private:
        NoCopy(const NoCopy &other) {}
        void operator=(const NoCopy &other) {}
    public:
        NoCopy() {}
        virtual ~NoCopy() {}

};

#endif
