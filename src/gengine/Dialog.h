#ifndef HEADER_DIALOG_H
#define HEADER_DIALOG_H

#include "NoCopy.h"
#include "Path.h"

#include "SDL_mixer.h"
#include <string>

/**
 * Dialog with sound and subtitles.
 */
class Dialog: public NoCopy {
    private:
        Mix_Chunk *m_sound;
        std::string m_lang;
        std::string m_subtitles;
    public:
        Dialog(const std::string &lang,
                const Path &soundfile, const std::string &subtitles);
        virtual ~Dialog();

        int talk() const;
        std::string getLang() const { return m_lang; }
        bool equalSound(const Mix_Chunk *other) const
        { return m_sound == other; }
};

#endif

