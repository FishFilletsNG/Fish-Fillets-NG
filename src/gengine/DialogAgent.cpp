#include "DialogAgent.h"

#include "OptionAgent.h"
#include "ResDialogPack.h"
#include "Dialog.h"
#include "PlannedDialog.h"


//-----------------------------------------------------------------
    void
DialogAgent::own_init()
{
    m_dialogs = new ResDialogPack();
}
//-----------------------------------------------------------------
/**
 * Run planned dialog.
 */
    void
DialogAgent::own_update()
{
    if (false == m_planned.empty()) {
        PlannedDialog *dialog = m_planned.front();
        if (dialog->getDelay() > 0) {
            dialog->decDelay();
        }
        else {
            dialog->talk();
            m_planned.pop_front();
            removeFirstNotTalking();
            m_running.push_back(dialog);
        }
    }
}
//-----------------------------------------------------------------
    void
DialogAgent::own_shutdown()
{
    removeAll();
    delete m_dialogs;
}
//-----------------------------------------------------------------
/**
 * Store new dialog.
 * @throws NameException when dialog with this name exists
 */
    void
DialogAgent::addDialog(const std::string &name, Dialog *dialog)
{
    m_dialogs->addRes(name, dialog);
}
//-----------------------------------------------------------------
/**
 * Run this dialog after delay cycles.
 * @param name dialog name
 * @param delay delay in cycles after last dialog
 * @param actor who is talking
 */
    void
DialogAgent::planDialog(const std::string &name, int delay, int actor)
{
    std::string lang = OptionAgent::agent()->getParam("lang", "english");
    Dialog *dialog = m_dialogs->findDialog(name, lang);
    if (dialog) {
        PlannedDialog *plan = new PlannedDialog(actor, delay, dialog);
        m_planned.push_back(plan);

        LOG_DEBUG(ExInfo("planned dialog")
                .addInfo("name", name)
                .addInfo("delay", delay)
                .addInfo("actor", actor));
    }
}
//-----------------------------------------------------------------
    bool
DialogAgent::isTalking(int actor)
{
    t_running::iterator end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != end; ++i) {
        if ((*i)->getActor() == actor && (*i)->isTalking()) {
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
/**
 * Remove first not talking dialog from m_running.
 */
void
DialogAgent::removeFirstNotTalking()
{
    t_running::iterator end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != end; ++i) {
        if (false == (*i)->isTalking()) {
            delete *i;
            m_running.erase(i);
            return;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Delete all dialogs, including planned dialogs.
 */
    void
DialogAgent::removeAll()
{
    t_planned::iterator plan_end = m_planned.end();
    for (t_planned::iterator i = m_planned.begin(); i != plan_end; ++i) {
        delete *i;
    }
    m_planned.clear();

    t_running::iterator run_end = m_running.end();
    for (t_running::iterator j = m_running.begin(); j != run_end; ++j) {
        delete *j;
    }
    m_running.clear();

    m_dialogs->removeAll();
}

