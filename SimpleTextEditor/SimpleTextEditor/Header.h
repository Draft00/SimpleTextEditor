#ifndef SIMPLE_TEXT_EDITOR
#define SIMPLE_TEXT_EDITOR

#include <vector>
#include <iostream>
#include <fstream>
#include "curses.h"
#include "MyString.h"
#include "panel.h"

class WindowModel;

class Observer
{
public:
    virtual void UpdateMode(const char* str) = 0;
    virtual void UpdateCmd(const char* str) = 0;
    virtual void UpdateLineStats(size_t curr_line, size_t lines) = 0;
    virtual void UpdateFilename(const char* filename) = 0;
    virtual void ClearCmd() = 0;
    virtual void EndCmd() = 0;
    virtual void PrintMessage(const char* str) = 0;
    virtual void mvPrintMessage(const char* str, int y, int x) = 0;
    virtual void PutYX(int* y, int* x) = 0;
    virtual void DoScroll(int n) = 0;

    //virtual void PressedZero() = 0;
    virtual void PressedKeyUp() = 0;
    //virtual void Pressedb() = 0;
    virtual void Pressedw() = 0;

    virtual void MoveInText(int y, int x) = 0;
};

class Observable
{
public:
    void AddObserver(Observer* observer);
    void NotifyUpdateMode(const char* str);
    void NotifyUpdateLineStats(size_t curr_line, size_t lines);
    void NotifyUpdateFilename(const char* filename);
    void NotifyUpdateCmd(const char* str);
    void NotifyPrintMsg(const char* str);
    void NotifymvPrintMsg(const char* str, int y, int x);
    void NotifyClearCmd();
    void NotifyEndCmd();

    void GetViewYX(int* y, int* x);

    void NotifyMoveCursor(int y, int x);
    void NotifyScroll(int n);

    void NotifyPressedKeyUp();
    void NotifyPressedb();
    void NotifyPressedw();


private:
    Observer* m_view_observer = nullptr; 
    std::vector<Observer*> _observers;
};

class WindowModel : public Observable
{
public:
    WindowModel();
    ~WindowModel();

    enum status {
        NORMAL = 0,
        COMMAND,
        NAVIGATION,
        SEARCH,
        WAITING,
        EXIT
    };
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
    int x = 0;
    int y = 0;
    int x_nav = 0;
    int IDX_LAST_LINE = 27;
    int IDX_LAST_COL = 99;

    bool flag_changes = false;

    void SetStatus(status new_status);
    void SetFilename(const char* new_filename);
    void SetStartConfig();

    int GetKeyFromCmd(int key);
    int GetKeyFromNavigation(int key);
    int ParseCommand();

    void OpenFile(STD::MyString s_filename);
    void SaveFile();
    void SaveFile(STD::MyString s_filename);
    void CountLines();
    void CountIdxLastLine(); 

    void ProcPressedw();

private:
    void m_ProcPressedKeyLeft();
    void m_ProcPressedKeyRight();
    void m_ProcPressedDollar();
    void m_ProcPressedZero();
    void m_ProcPressedKeyDown();
    void m_ProcPressedKeyUp();

    void m_GetYX(int* y, int* x);

    bool m_CheckScrollDown() const;
    bool m_CheckScrollUp() const;
    bool m_ScrollDown(int curr_pos, int n);
    bool m_ScrollUp(int curr_pos, int n);
    void m_GotoXNav();
    size_t m_CountIdxFirstLineDown(int n) const;
    size_t m_CountIdxFirstLineUp(int n) const;
    size_t m_DropSpace(size_t idx_) const;
    size_t m_FindSymbol(size_t idx) const;

    size_t m_ReversFind(const char* str, size_t start_idx) const;
    size_t m_reverse_find(const char* str, size_t start_idx, size_t len) const;
    bool m_find_compare(const char* str, size_t len, size_t pos) const;
};

class ConsoleView : public Observer
{
public:
    ConsoleView(WindowModel* model);
    ~ConsoleView();


    void UpdateMode(const char* str_mode) override;
    void UpdateCmd(const char* str) override;
    void UpdateLineStats(size_t curr_line, size_t lines) override;
    void UpdateFilename(const char* filename) override;

    void ClearCmd() override;
    void EndCmd() override;
    void PrintMessage(const char* str) override;
    void mvPrintMessage(const char* str, int y, int x) override;
    void PressedKeyUp() override; 

   // void Pressedb() override;
    void Pressedw() override;

    void MoveInText(int y, int x) override;
    void DoScroll(int n) override;

    void PutYX(int* y, int* x) override;

    WINDOW* cmd_win = nullptr;
    WINDOW* text_win = nullptr;
    WINDOW* mode_win = nullptr;
    WINDOW* filename_win = nullptr;
    WINDOW* line_stats_win = nullptr;
    WINDOW* help_win = nullptr;
    PANEL* help_pannel = nullptr;
    PANEL* text_pannel = nullptr;

private:
    WindowModel* m_mymodel;

    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;

    int TEXT_W_LINES = 28;
    int TEXT_W_COLS = 100;

    int y_start_cmd = 0;
    int x_start_cmd = 0;
    int y = 0;
    int x = 0;
    int new_y = 0;
    int y_nav = 0;
    int x_nav = 0;


    WINDOW* create_text_win();
    WINDOW* create_cmd_win();

    void create_status_wins();
    void init_coloros_pair();
    void scroll_down(int curr_pos);
    void find_new_x_pos(int* curr_pos);
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

private:
    WindowModel* m_mymodel;
    ConsoleView* m_view; 

    int m_choice = 0;
};
#endif // !SIMPLE_TEXT_EDITOR
