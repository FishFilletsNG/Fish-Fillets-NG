#ifndef HEADER_MOUSECONTROL_H
#define HEADER_MOUSECONTROL_H

class V2;
class Controls;
class View;
class InputProvider;
class FinderAlg;

/**
 * Mouse controls move.
 */
class MouseControl {
    private:
        Controls *m_controls;
        const View *m_view;
        FinderAlg *m_finder;
    private:
        bool moveTo(const V2 &field) const;
        bool moveHardTo(const V2 &field) const;
    public:
        MouseControl(Controls *controls, const View *m_view,
                FinderAlg *finder);
        bool mouseDrive(const InputProvider *input) const;
};

#endif
