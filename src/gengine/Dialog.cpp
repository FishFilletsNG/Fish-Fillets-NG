#include "Dialog.h"

#include "ResSoundAgent.h"
#include "SoundAgent.h"

//-----------------------------------------------------------------
Dialog::Dialog(const std::string &lang,
        const Path &soundfile, const std::string &subtitles)
{
    m_lang = lang;
    m_subtitles = subtitles;
    m_sound = ResSoundAgent::agent()->loadSound(soundfile);
}
//-----------------------------------------------------------------
Dialog::~Dialog()
{
    Mix_FreeChunk(m_sound);
}
//-----------------------------------------------------------------
/**
 * Run dialog.
 * @return channel number where the sound is played or -1
 */
    int
Dialog::talk() const
{
    int channel = SoundAgent::agent()->playSound(m_sound);
    //TODO: run subtitles
    LOG_INFO(ExInfo("TEST: subtitles")
            .addInfo("content", m_subtitles));
    return channel;
}

