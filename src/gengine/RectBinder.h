#ifndef HEADER_RECTBINDER_H
#define HEADER_RECTBINDER_H

class BaseMsg;

#include "NoCopy.h"
#include "SDL.h"


#include <list>
#include <string>

/**
 * Rect binder.
 */
class RectBinder : public NoCopy {
    private:
        typedef std::list<std::pair<SDL_Rect,BaseMsg*> > t_rects;
        t_rects m_rects;
    private:
        std::string rectToString(const SDL_Rect &rect) const;
    public:
        ~RectBinder();

        void addRect(const SDL_Rect &rect, BaseMsg *msg);

        void lbuttonDown(const SDL_MouseButtonEvent &button);
};

#endif
