#ifndef HEADER_DIALOG_H
#define HEADER_DIALOG_H

#include "NoCopy.h"
#include "Path.h"

#include "SDL_mixer.h"
#include <string>

/**
 * Dialog with sound and subtitle.
 */
class Dialog: public NoCopy {
    private:
        Mix_Chunk *m_sound;
        std::string m_lang;
        std::string m_subtitle;
    protected:
        virtual void runSubtitle() const;
    public:
        Dialog(const std::string &lang,
                const Path &soundfile, const std::string &subtitle);
        virtual ~Dialog();

        int talk() const;
        std::string getLang() const { return m_lang; }
        std::string getSubtitle() const { return m_subtitle; }

        bool equalSound(const Mix_Chunk *other) const
        { return m_sound == other; }
};

#endif

