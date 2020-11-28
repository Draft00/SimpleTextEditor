#include "Header.h"

void Observable::AddObserver(Observer* observer)
{
    m_view_observer = observer;
}

void Observable::GetViewYX(int* y, int* x)
{
    m_view_observer->PutYX(y, x);
}
void Observable::NotifyScroll(int n)
{
    m_view_observer->DoScroll(n);
}
void Observable::NotifyUpdateMode(const char* str)
{
    m_view_observer->UpdateMode(str);
}

void Observable::NotifyUpdateFilename(const char* filename)
{
    m_view_observer->UpdateFilename(filename);
}
void Observable::NotifyUpdateLineStats(size_t curr_line, size_t lines)
{
    m_view_observer->UpdateLineStats(curr_line, lines);
}

void Observable::NotifyUpdateCmd(const char* str)
{
    m_view_observer->UpdateCmd(str);
}
void Observable::NotifyPrintMsg(const char* str)
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

void Observable::NotifyPressedKeyUp()
{
    m_view_observer->PressedKeyUp();
}

void Observable::NotifyPressedb()
{
    //m_view_observer->Pressedb();
}
void Observable::NotifyPressedw()
{
    m_view_observer->Pressedw();
}

void Observable::NotifyMoveCursor(int y, int x)
{
    m_view_observer->MoveInText(y, x);
}