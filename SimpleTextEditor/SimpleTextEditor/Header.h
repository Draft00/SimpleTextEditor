#ifndef SIMPLE_TEXT_EDITOR
#define SIMPLE_TEXT_EDITOR

#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include "MyString.h"
#include "AdapterLib.h"

#define LAST_IDX_LINE 1
#define FIRST_IDX_LINE 0

enum status {
    NORMAL = 0,
    COMMAND,
    NAVIGATION,
    SEARCH,
    WAITING,
    EXIT
};

class WindowModel;
class Controller; 

class Observer
{
public:
    virtual void UpdateMode(const STD::MyString& mode, status new_status) = 0;
    virtual void UpdateFilename(const STD::MyString& filename) = 0;
    virtual void UpdateCmd(const STD::MyString& str) = 0;

    virtual void UpdateLineStats(size_t curr_line, size_t lines) = 0;
    virtual void ClearCmd() = 0;
    virtual void EndCmd() = 0;
    virtual void PrintMessage(const STD::MyString& str) = 0;
    virtual void mvPrintMessage(const char* str, int y, int x) = 0;
    virtual void UpdateVector(const STD::MyString& str) = 0;
    virtual void PressedKeyUp() = 0;

    virtual void KeyNavigation(const STD::MyString& str, size_t idx, int command) = 0;
};

class Observable
{
public:
    void AddObserver(Observer* observer);
    void NotifyUpdateMode(const STD::MyString& mode, status new_status);
    void NotifyUpdateLineStats(size_t curr_line, size_t lines);
    void NotifyUpdateFilename(const STD::MyString& filename);
    void NotifyUpdateCmd(const STD::MyString& str);
    void NotifyPrintMsg(const STD::MyString& str);
    void NotifymvPrintMsg(const char* str, int y, int x);
    void NotifyClearCmd();
    void NotifyEndCmd();
    void NotifyUpdateVector(const STD::MyString& str);


    void NotifyMoveCursor(int y, int x);

    void NotifyPressedKeyUp();
    void NotifyPressedb();
    void NotifyPressedw();

    void SendNavigation(const STD::MyString& str, size_t idx, int command);
private:
    Observer* m_view_observer = nullptr; 
};

class WindowModel : public Observable
{
public:
    WindowModel();
    ~WindowModel();

    enum smd_command
    {
        STOP = 2
    };

    status curr_status = WAITING;
    STD::MyString filename = "none";
    size_t num_curr_line = 0;
    size_t num_lines = 0;
    std::string mode_str[5] = { "NRM", "CMD", "NAV", "SRC", "..." };

    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;

    STD::MyString str;
    STD::MyString file_data;

    size_t idx_first_line = 0;
    size_t idx_last_line = 0;
    size_t idx = 0;
    size_t temp_idx = 0; 
    int x = 0;
    int y = 0;

    int x_nav = 0;
    int IDX_LAST_LINE = 27;
    int IDX_LAST_COL = 99;
    int TEXT_W_LINES = 28;
    int TEXT_W_COLS = 100;

    bool flag_changes = false;

    void SetStatus(status new_status);
    void SetFilename(const char* new_filename);
    void SetStartConfig();

    void GetKeyFromController(int key);

    int GetKeyFromCmd(int key);
    int GetKeyFromNavigation(int key);
    int ParseCommand();

    void OpenFile(STD::MyString s_filename);
    void SaveFile();
    void SaveFile(STD::MyString s_filename);
    void CountLines();

    void ProcPressedw();

private:
    void m_ProcPressedKeyLeft();
    void m_ProcPressedKeyRight();
    void m_ProcPressedDollar();
    void m_ProcPressedZero();
    void m_ProcPressedKeyDown();
    void m_ProcPressedKeyUp();

    void m_ProcKeyInWaiting(int key);
    void m_ProcKeyInNavigation(int key);

    bool m_CheckScrollDown() const;
    bool m_CheckScrollUp() const;
    bool m_ScrollDown(int curr_pos, int n);
    bool m_ScrollUp(int curr_pos, int n);
    void m_GotoXNav();
    size_t m_CountIdxFirstLineDown(int n) const;
    size_t m_CountIdxFirstLineUp(int n) const;
    size_t m_DropSpace(size_t idx_) const;
    size_t m_FindSymbol(size_t idx) const;
    size_t m_FindStartIdxLine();

    size_t m_ReversFind(const char* str, size_t start_idx) const;
    size_t m_reverse_find(const char* str, size_t start_idx, size_t len) const;
    bool m_find_compare(const char* str, size_t len, size_t pos) const;
};

class ConsoleView : public Observer
{
public:
    ConsoleView();
    ~ConsoleView();
    void AddController(Controller* Controller);
    void AddAdapter(AdapterPDCurses* Adapter);

    void START();
    void SetStartConfig();

    void UpdateMode(const STD::MyString& mode, status new_status) override;
    void UpdateCmd(const STD::MyString& str) override;
    void UpdateLineStats(size_t curr_line, size_t lines) override;
    void UpdateFilename(const STD::MyString& filename) override;
    void UpdateVector(const STD::MyString& str) override;

    void PutModelNewIdx(size_t new_idx);
    void KeyNavigation(const STD::MyString& str, size_t idx, int command) override;

    void ClearCmd() override;
    void EndCmd() override;
    void PrintMessage(const STD::MyString& str) override;
    void mvPrintMessage(const char* str, int y, int x) override;
    void PressedKeyUp() override; 

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
    int TEXT_W_COLS = 100;

    size_t new_idx = 0;

    int y = 0;
    int x = 0;
    int table_y = 0;
    int m_TableYFirstLine = 0;
    int new_y = 0;
    int y_nav = 0;
    int x_nav = 0;

    void m_ProcPressedKeyLeft();
    void m_ProcPressedKeyRight();
    void m_ProcPressedDollar(const STD::MyString& str, size_t idx); //мб удалить (не используются). 
    void m_ProcPressedZero();
    void m_ProcPressedKeyDown(const STD::MyString& str, size_t idx);
    void m_ProcPressedKeyUp(const STD::MyString& str, size_t idx);
    void m_ProcPressedKeygg(const STD::MyString& str, size_t idx);

    WINDOW* m_create_text_win();
    WINDOW* m_create_cmd_win();
    void m_create_status_wins();
    void m_init_coloros_pair();

};

class Controller
{
public:
    Controller(WindowModel* model, ConsoleView* view) {
        m_mymodel = model;
        m_view = view;
    }
    ~Controller() {};
    void start();

    void GetKeyFromView(int key);
    void PutModelNewIdx(size_t idx);

    void TEST() {
        m_mymodel->OpenFile("test file2.txt");
        m_mymodel->NotifyPrintMsg(m_mymodel->file_data);
        m_mymodel->curr_status = NAVIGATION;
        m_mymodel->NotifyUpdateVector(m_mymodel->file_data);
    }
private:
    WindowModel* m_mymodel;
    ConsoleView* m_view; 

    int m_choice = 0;
};
#endif // !SIMPLE_TEXT_EDITOR
