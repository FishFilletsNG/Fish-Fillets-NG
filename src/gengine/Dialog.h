#ifndef HEADER_DIALOG_H
#define HEADER_DIALOG_H

#include "NoCopy.h"
#include "StringTool.h"

#include "SDL_mixer.h"
#include <string>

/**
 * Dialog with sound and subtitle.
 */
class Dialog: public NoCopy {
    public:
        static const std::string DEFAULT_LANG;
    private:
        Mix_Chunk *m_sound;
        std::string m_soundfile;
        std::string m_lang;
        std::string m_subtitle;
    public:
        Dialog(const std::string &lang,
                const std::string &soundfile, const std::string &subtitle);
        virtual ~Dialog();

        int talk(int volume, int loops=0);
        virtual void runSubtitle(const StringTool::t_args &args) const;
        std::string getLang() const { return m_lang; }
        std::string getSubtitle() const { return m_subtitle; }
        std::string getFormatedSubtitle(const StringTool::t_args &args) const;
        int getMinTime() const;

        bool equalSound(const Mix_Chunk *other) const
        { return m_sound == other; }
};

#endif

