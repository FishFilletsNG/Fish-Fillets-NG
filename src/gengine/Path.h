#ifndef HEADER_PATH_H
#define HEADER_PATH_H

#include <string>

/**
 * Path to a installed data.
 */
class Path {
    private:
        std::string m_path;
    private:
        static Path dataPath(const std::string &file, bool writeable);
        explicit Path(const std::string &file);
    public:
        static Path dataReadPath(const std::string &file);
        static Path dataWritePath(const std::string &file);

        static Path dataSystemPath(const std::string &file);
        static Path dataUserPath(const std::string &file);

        std::string getPosixName() const { return m_path; }
        std::string getNative() const;
        bool exists() const;
};

#endif

