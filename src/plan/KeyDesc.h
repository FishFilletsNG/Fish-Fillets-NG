#ifndef HEADER_KEYDESC_H
#define HEADER_KEYDESC_H

#include <string>

/**
 * Description of control key.
 */
class KeyDesc {
    private:
        int m_index;
        std::string m_desc;
    public:
        KeyDesc(int index, const std::string &desc);

        int getIndex() const { return m_index; }
        std::string getDesc() const { return m_desc; }
};

#endif
