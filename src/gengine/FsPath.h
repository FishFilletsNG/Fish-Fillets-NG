#ifndef HEADER_FSPATH_H
#define HEADER_FSPATH_H

#include <string>

/**
 * File system path.
 * System dependent operations.
 */
class FsPath {
    public:
        static std::string getNative(const std::string &file);
        static bool exists(const std::string &file);
        static std::string join(const std::string &dir,
                const std::string &file);
        static void createPath(const std::string &dir);
};

#endif

