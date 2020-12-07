#include "Header.h"

void Observable::AddObserver(Observer* observer) {
    m_view_observer = observer;
}

void Observable::NotifyUpdateMode(const STD::MyString& str, status new_status) {
    m_view_observer->UpdateMode(str, new_status);
}

void Observable::NotifyUpdateFilename(const STD::MyString& filename) {
    m_view_observer->UpdateFilename(filename);
}

void Observable::NotifyUpdateLineStats() {
    m_view_observer->UpdateLineStats();
}

void Observable::NotifyUpdateCmd(const STD::MyString& str) {
    m_view_observer->UpdateCmd(str);
}

void Observable::NotifyUpdateSearch(const STD::MyString& str) {
    m_view_observer->UpdateSearch(str);
}

void Observable::NotifyPrintMsg(const STD::MyString& str) {
    m_view_observer->PrintMessage(str);
}

void Observable::NotifyPrintLineByLine(const STD::MyString& str, int y_start, int x_start) {
    m_view_observer->PrintLineByLine(str, y_start, x_start);
}

void Observable::NotifyPrintLineByLineXY(const STD::MyString& str, int y_start, int x_start, int offset) {
    m_view_observer->PrintLineByLineXY(str, y_start, x_start, offset);
}

void Observable::NotifyJumpTo(const STD::MyString& str, size_t idx, const STD::MyString& line) {
    m_view_observer->JumpTo(str, idx, line);
}

void Observable::NotifyGetLastFirstIdx() {
    m_view_observer->PutLastFirstIdx();
}

void Observable::NotifymvPrintMsg(const char* str, int y, int x) {
    m_view_observer->mvPrintMessage(str, y, x);
}

void Observable::NotifyClearCmd() {
    m_view_observer->ClearCmd();
}

void Observable::NotifyEndCmd() {
    m_view_observer->EndCmd();
}

void Observable::NotifyUpdateVector(const STD::MyString& str) {
    m_view_observer->UpdateVector(str);
}

void Observable::SendNavigation(const STD::MyString& str, size_t idx, int command) {
    m_view_observer->KeyNavigation(str, idx, command);
}

void Observable::NotifyMoveCursorToIdx(const STD::MyString& str, size_t idx) {
    m_view_observer->MoveCursorToIdx(str, idx);
}

void Observable::NotifyIdxForMove(const STD::MyString& str, size_t idx) {
    m_view_observer->SetIdxForMove(str, idx);
}