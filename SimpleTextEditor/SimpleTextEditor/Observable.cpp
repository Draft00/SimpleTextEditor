#include "Header.h"

void Observable::AddObserver(Observer* observer)
{
    m_view_observer = observer;
}

void Observable::NotifySetStartConfig()
{
    m_view_observer->USetStartConfig();
}

void Observable::NotifyUpdateMode()
{
    m_view_observer->UpdateMode();
}

void Observable::NotifyUpdateFilename()
{
    m_view_observer->UpdateFilename();
}

void Observable::NotifyUpdateCmd()
{
    m_view_observer->UpdateCmd();
}

void Observable::NotifyClearCmd()
{
    m_view_observer->ClearCmd();
}
void Observable::NotifyEndCmd()
{
    m_view_observer->EndCmd();
}