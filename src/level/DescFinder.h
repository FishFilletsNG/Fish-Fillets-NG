#ifndef HEADER_DESCFINDER_H
#define HEADER_DESCFINDER_H

class Level;

#include <string>

/**
 * Allows to find level name and description.
 */
class DescFinder {
    public:
        virtual ~DescFinder() {}
        virtual std::string findLevelName(
                const std::string &codename) const = 0;
        virtual std::string findDesc(
                const std::string &codename) const = 0;
};

#endif
