#ifndef HEADER_STRINGTOOL_H
#define HEADER_STRINGTOOL_H

#include <string>

/**
 * String utils.
 */
class StringTool {
    public:
        static long readInt(const char *text, bool *ok);
        static std::string toString(long value);
};

#endif

