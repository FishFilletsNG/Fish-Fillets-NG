#ifndef HEADER_PATH_H
#define HEADER_PATH_H

#include <string>
#include "boost/filesystem/path.hpp"

/**
 * Path to a installed data.
 */
class Path {
    private:
        boost::filesystem::path m_path;
    private:
        static Path dataPath(const std::string &file, bool writeable);
    protected:
        Path(const boost::filesystem::path &file);
    public:
        static Path dataReadPath(const std::string &file);
        static Path dataWritePath(const std::string &file);

        static Path dataSystemPath(const std::string &file);
        static Path dataUserPath(const std::string &file);

        static bool isValid(const std::string &file);

        std::string getNative() const;
        std::string getBasename() const;
        bool exists() const;

};

#endif

