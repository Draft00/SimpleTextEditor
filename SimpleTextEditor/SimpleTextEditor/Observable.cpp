#include "Header.h"

void Observable::AddObserver(Observer* observer)
{
    m_view_observer = observer;
}

void Observable::NotifyUpdateMode(const STD::MyString& str, status new_status)
{
    m_view_observer->UpdateMode(str, new_status);
}

void Observable::NotifyUpdateFilename(const STD::MyString& filename)
{
    m_view_observer->UpdateFilename(filename);
}
void Observable::NotifyUpdateLineStats(size_t curr_line, size_t lines)
{
    m_view_observer->UpdateLineStats(curr_line, lines);
}

void Observable::NotifyUpdateCmd(const STD::MyString& str)
{
    m_view_observer->UpdateCmd(str);
}
void Observable::NotifyPrintMsg(const STD::MyString& str)
{
    m_view_observer->PrintMessage(str);
}
void Observable::NotifymvPrintMsg(const char* str, int y, int x)
{
    m_view_observer->mvPrintMessage(str, y, x);
}

void Observable::NotifyClearCmd()
{
    m_view_observer->ClearCmd();
}
void Observable::NotifyEndCmd()
{
    m_view_observer->EndCmd();
}
void Observable::NotifyUpdateVector(const STD::MyString& str)
{
    m_view_observer->UpdateVector(str);
}
void Observable::SendNavigation(const STD::MyString& str, size_t idx, int command)
{
    m_view_observer->KeyNavigation(str, idx, command);
}
void Observable::NotifyPressedKeyUp()
{
    m_view_observer->PressedKeyUp();
}

void Observable::NotifyPressedb()
{
    //m_view_observer->Pressedb();
}

void Observable::NotifyMoveCursor(int y, int x)
{
    //m_view_observer->MoveInText(y, x);
}