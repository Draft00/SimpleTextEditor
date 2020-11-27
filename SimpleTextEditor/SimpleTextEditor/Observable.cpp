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
void Observable::NotifyPrintNewText()
{
    m_view_observer->PrintNewText();
}
void Observable::NotifyPressedDollar()
{
    m_view_observer->PressedDollar();
}
void Observable::NotifyPressedZero()
{
    m_view_observer->PressedZero();
}
void Observable::NotifyPressedKeyDown()
{
    m_view_observer->PressedKeyDown();
}
void Observable::NotifyPressedB()
{
    m_view_observer->PressedB();
}
void Observable::NotifyPressedW()
{
    m_view_observer->PressedW();
}
void Observable::NotifyPressedKeyUp()
{
    m_view_observer->PressedKeyUp();
}