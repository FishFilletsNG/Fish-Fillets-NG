#ifndef HEADER_CONTROLSYM_H
#define HEADER_CONTROLSYM_H

/**
 * Control symbols to save.
 * Symbol cannot be '\0'.
 */
class ControlSym {
    private:
        char m_up;
        char m_down;
        char m_left;
        char m_right;
    public:
        ControlSym(char up, char down, char left, char right)
            : m_up(up), m_down(down), m_left(left), m_right(right)
        {}

        char getUp() const { return m_up; }
        char getDown() const { return m_down; }
        char getLeft() const { return m_left; }
        char getRight() const { return m_right; }
};

#endif
