#ifndef HEADER_DIALOG_H
#define HEADER_DIALOG_H

#include "NoCopy.h"
#include "StringTool.h"

#include "SDL_mixer.h"
#include <string>

/**
 * Dialog with sound and subtitle.
 * Dialog is const class only sound is lazy loaded.
 */
class Dialog: public NoCopy {
    public:
        static const std::string DEFAULT_LANG;
    private:
        mutable Mix_Chunk *m_sound;
        std::string m_soundfile;
        std::string m_lang;
        std::string m_subtitle;
    public:
        Dialog(const std::string &lang,
                const std::string &soundfile, const std::string &subtitle);
        virtual ~Dialog();

        bool isSpeechless() const { return m_soundfile.empty(); }
        int talk(int volume, int loops=0) const;
        virtual void runSubtitle(const StringTool::t_args &args) const;
        std::string getLang() const { return m_lang; }
        std::string getSubtitle() const { return m_subtitle; }
        std::string getFormatedSubtitle(const StringTool::t_args &args) const;
        int getMinTime() const;

        bool equalSound(const Mix_Chunk *other) const
        { return m_sound == other; }
};

#endif

