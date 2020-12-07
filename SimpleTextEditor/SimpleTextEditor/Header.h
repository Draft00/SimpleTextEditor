#ifndef SIMPLE_TEXT_EDITOR
#define SIMPLE_TEXT_EDITOR

#include <vector>
#include <iostream>
#include <fstream>
#include "MyString.h"
#include "AdapterLib.h"

#define FIRST_IDX_LINE 0
#define LAST_IDX_LINE 1

enum status {
    NORMAL = 0,
    COMMAND,
    NAVIGATION,
    SEARCH,
    WAITING,
    EXIT,
    HELP
};

class WindowModel;
class Controller; 

class Observer
{
public:
    virtual void UpdateMode(const STD::MyString& mode, status new_status) = 0;
    virtual void UpdateFilename(const STD::MyString& filename) = 0;
    virtual void UpdateCmd(const STD::MyString& str) = 0;
    virtual void UpdateSearch(const STD::MyString& str) = 0;
    virtual void UpdateLineStats() = 0;
    virtual void ClearCmd() = 0;
    virtual void EndCmd() = 0;
    virtual void PrintMessage(const STD::MyString& str) = 0;
    virtual void mvPrintMessage(const char* str, int y, int x) = 0;
    virtual void UpdateVector(const STD::MyString& str) = 0;
    virtual void PrintLineByLine(const STD::MyString& str, int y_start, int x_start) = 0;
    virtual void PrintLineByLineXY(const STD::MyString& str, int y_start, int x_start, int offset) = 0;
    virtual void MoveCursorToIdx(const STD::MyString& str, size_t idx) = 0;
    virtual void PutLastFirstIdx() = 0;
    virtual void KeyNavigation(const STD::MyString& str, size_t idx, int command) = 0;
    virtual void JumpTo(const STD::MyString& str, size_t idx, const STD::MyString& line) = 0;
    virtual void NewText() = 0;
    virtual void SetIdxForMove(const STD::MyString& str, size_t idx_move) = 0;
};

//NOTE: it should be an _INTERFACE_ with pure virtual functions
//but Julia doesn't like having a lot of code, so she implemented these methods (this is not what she wanted).
//Julia thinks it should look like an interface Observer.
class Observable
{
public:
    void AddObserver(Observer* observer);
    void NotifyUpdateMode(const STD::MyString& mode, status new_status);
    void NotifyUpdateFilename(const STD::MyString& filename);
    void NotifyUpdateCmd(const STD::MyString& str);
    void NotifyUpdateSearch(const STD::MyString& str);
    void NotifyUpdateLineStats();
    void NotifyPrintMsg(const STD::MyString& str);
    void NotifymvPrintMsg(const char* str, int y, int x);
    void NotifyClearCmd();
    void NotifyEndCmd();
    void NotifyUpdateVector(const STD::MyString& str);
    void NotifyPrintLineByLine(const STD::MyString& str, int y_start, int x_start);
    void NotifyPrintLineByLineXY(const STD::MyString& str, int y_start, int x_start, int offset);
    void NotifyMoveCursorToIdx(const STD::MyString& str, size_t idx);
    void NotifyGetLastFirstIdx();
    void NotifyJumpTo(const STD::MyString& str, size_t idx, const STD::MyString& line);
    void NotifyIdxForMove(const STD::MyString& str, size_t idx);

    void SendNavigation(const STD::MyString& str, size_t idx, int command);
private:
    Observer* m_view_observer = nullptr; 
};

class WindowModel : public Observable
{
public:
    WindowModel();
    ~WindowModel();

    enum cmd_command
    {
        STOP = 2
    };

    status curr_status = WAITING;
    STD::MyString filename = "none";
    STD::MyString mode_str[7] = { "NRM", "CMD", "NAV", "SRC", "...", "...", "HLP"};
    STD::MyString command_NG;
    STD::MyString copy_str;

    STD::MyString str;
    STD::MyString search_str; 
    STD::MyString memory_search_str;
    STD::MyString file_data;
    STD::MyString help_str;

    size_t idx = 0;

    bool flag_changes = false;
    size_t LastIdxCopyDel = 0;
    size_t FirstIdxCopyDel = 0;

    void SetStatus(status new_status);
    void SetFilename(const char* new_filename);

    void GetKeyFromController(int key);
    void GetSymbolReplace(int c);

    int GetKeyFromCmd(int key);
    int GetKeyFromNavigation(int key);
    int GetKeyFromNormal(int key);
    int GetKeyFromSearch(int key);
    int GetKeyFromHelp(int key);

    int ParseCommand();

    void OpenFile(STD::MyString s_filename);
    void SaveFile(STD::MyString s_filename);
    void OpenHelp();

private:
    void m_ProcPressedi();
    void m_ProcPressedx();
    void m_ProcKeyInWaiting(int key);
    void m_ProcPreseedS();
    void m_ProcPreseedd();
    void m_ProcPreseedy();
    void m_ProcPressedp();
    void m_ProcPressedc();
    void m_ProcPressedv();
    void m_ProcPressedw();
    void m_ProcPressedb();
    void m_DeleteLine(size_t first_idx, size_t last_idx);
    void m_FindOneWord(size_t* left, size_t* right, size_t temp_idx);
    bool m_ProcPressedSearchEnd(size_t idx, STD::MyString search_str);
    bool m_ProcPressedSearchBegin(size_t idx, STD::MyString search_str);
    bool m_IsLetterASCII(int ch);

    size_t m_ReversFind(const char* str, size_t start_idx) const;
    size_t m_reverse_find(const char* str, size_t start_idx, size_t len) const;
    bool m_find_compare(const char* str, size_t len, size_t pos) const;

};

class ConsoleView : public Observer
{
public:
    ConsoleView(AdapterPDCurses* Adapter);
    ~ConsoleView();
    void AddController(Controller* Controller);
    //void AddAdapter(AdapterPDCurses* Adapter);

    void START();
    void SetStartConfig();

    void UpdateMode(const STD::MyString& mode, status new_status) override;
    void UpdateCmd(const STD::MyString& str) override;
    void UpdateSearch(const STD::MyString& str) override;
    void UpdateLineStats() override;
    void UpdateFilename(const STD::MyString& filename) override;
    void UpdateVector(const STD::MyString& str) override;
    void SetIdxForMove(const STD::MyString& str, size_t idx_move) override;

    void PutModelNewIdx(size_t new_idx);
    void GetIdxForMove();
    void KeyNavigation(const STD::MyString& str, size_t idx, int command) override;
    void MoveCursorToIdx(const STD::MyString& str, size_t idx) override;

    void ClearCmd() override;
    void EndCmd() override;
    void PrintMessage(const STD::MyString& str) override;
    void mvPrintMessage(const char* str, int y, int x) override;

    void PrintLineByLine(const STD::MyString& str, int y_start, int x_start) override;
    void PrintLineByLineXY(const STD::MyString& str, int y_start, int x_start, int offset) override;
    void JumpTo(const STD::MyString& str, size_t idx, const STD::MyString& line) override;
    void PutLastFirstIdx() override;
    void NewText() override;

    void PutModelReplace(int c);

    void TEST_NAVI();

    WINDOW* cmd_win = nullptr;
    WINDOW* text_win = nullptr;
    WINDOW* mode_win = nullptr;
    WINDOW* filename_win = nullptr;
    WINDOW* line_stats_win = nullptr;
    WINDOW* help_win = nullptr;

    WINDOW* curr_win = nullptr;

    PANEL* help_pannel = nullptr;
    PANEL* text_pannel = nullptr;

    status curr_status = WAITING;

private:
    Controller* m_myController = nullptr; 
    AdapterPDCurses* m_myAdapter = nullptr;
    std::vector<std::vector<int>> table;

    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;

    int TEXT_W_LINES = 28;
    int LAST_Y = 27;
    int LAST_X = 99;
    int TEXT_W_COLS = 100;

    size_t new_idx = 0;

    int y = 0;
    int x = 0;
    int m_TableYFirstLine = 0;
    int x_nav = 0;

    void m_ProcPressedKeyLeft(size_t idx);
    void m_ProcPressedKeyRight(size_t idx);
    void m_ProcPressedDollar();
    void m_ProcPressedZero();
    void m_ProcPressedKeyDown(const STD::MyString& str, size_t idx);
    void m_ProcPressedKeyUp(const STD::MyString& str, size_t idx);
    void m_ProcPressedgg(const STD::MyString& str, size_t idx);
    void m_ProcPressedG(const STD::MyString& str, size_t idx);
    void m_ProcPressedKeyPGUP(const STD::MyString& str, size_t idx);
    void m_ProcPressedKeyPGDN(const STD::MyString& str, size_t idx);
    void m_ScrollForNewLine(const STD::MyString& str, size_t idx, size_t line);

    WINDOW* m_create_text_win();
    WINDOW* m_create_cmd_win();
    void m_create_status_wins();
    void m_init_coloros_pair();

};

class Controller
{
public:
    Controller(WindowModel* model);
    ~Controller() {};

    void GetKeyFromView(int key);
    void PutModelNewIdx(size_t idx);
    void PutModelFirstLastIdx(size_t first_idx, size_t last_idx);
    void GetIdxForMove();
    void PutModelReplace(int c);

    void TEST() {
        m_myModel->OpenFile("test file2.txt");
        m_myModel->NotifyUpdateVector(m_myModel->file_data);
        m_myModel->NotifyUpdateLineStats();
        m_myModel->NotifyPrintLineByLine(m_myModel->file_data, 0, 0);
        m_myModel->SetStatus(NAVIGATION);
        m_myModel->command_NG.clear();
    }
private:
    WindowModel* m_myModel;
};
#endif // !SIMPLE_TEXT_EDITOR
