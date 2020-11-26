#ifndef SIMPLE_TEXT_EDITOR
#define SIMPLE_TEXT_EDITOR

#include <vector>
#include <iostream>
#include <fstream>
#include "curses.h"
#include "MyString.h"
#include "panel.h"

class Observer
{
public:
    virtual void UpdateMode() = 0;
    virtual void UpdateCmd() = 0;
    virtual void UpdateLineStats() = 0;
    virtual void USetStartConfig() = 0;
    virtual void UpdateFilename() = 0;
    virtual void ClearCmd() = 0;
    virtual void EndCmd() = 0;
    virtual void PrintNewText() = 0;
    virtual void CountLines() = 0;
};

class Observable
{
public:
    void AddObserver(Observer* observer);
    void NotifySetStartConfig();
    void NotifyUpdateMode();
    void NotifyUpdateFilename();
    void NotifyDeleteSymbol();
    void NotifyUpdateCmd();
    void NotifyClearCmd();
    void NotifyEndCmd();
    void NotifyPrintNewText();

    void notifyUpdate_cmd_line()
    {
        int size = _observers.size();
        for (int i = 0; i < size; i++)
        {
            _observers[i]->UpdateCmd();
        }

    }
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

    STD::MyString str;
    STD::MyString file_data;
    //std::string str;
    char buffer[560] = {0};
    int idx = 0;

    void SetStatus(status new_status);
    void SetFilename(const char* new_filename);
    void SetStartConfig();

    int GetKeyFromCmd(int key);
    int GetKeyFromNavigation(int key);

    int ParseCommand();
    void OpenFile(STD::MyString s_filename);
    void SaveInFileAndExit();

    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;

    bool flag_changes = 0;

private:
    float _temperatureF;
    WINDOW* m_local_win = NULL;


};

class ConsoleView : public Observer
{
public:
    ConsoleView(WindowModel* model);
    ~ConsoleView();


    void UpdateMode() override;
    void UpdateCmd() override;
    void USetStartConfig() override;
    void UpdateFilename() override;
    void ClearCmd() override;
    void EndCmd() override;
    void PrintNewText() override;
    void CountLines() override;
    void UpdateLineStats() override;


    WINDOW* cmd_win = nullptr;
    WINDOW* text_win = nullptr;
    WINDOW* mode_win = nullptr;
    WINDOW* filename_win = nullptr;
    WINDOW* line_stats_win = nullptr;

private:
    WindowModel* m_mymodel;

    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;

    int TEXT_W_LINES = 28;
    int TEXT_W_COLS = 100;

    int y_start_cmd = 0;
    int x_start_cmd = 0;

    std::string mode_str[5] = { "NRM", "CMD", "NAV", "SRC", "..." };

    WINDOW* create_text_win();
    WINDOW* create_cmd_win();

    void create_status_wins();
    void init_coloros_pair();
};

class Controller
{
public:
    Controller(WindowModel* model, ConsoleView* view)
    {
        m_mymodel = model;
        m_view = view;
    }
    void start();

private:
    WindowModel* m_mymodel;
    ConsoleView* m_view; 

    int m_x = 0;
    int m_y = 0;
    int m_choice = 0;

    void presssed_up();
    void pressed_down(); //TODO SKROLLING
    void pressed_right();
    void pressed_left();
    void pressed_beg_line();
    void pressed_end_line();

    void get_filename();
};
#endif // !SIMPLE_TEXT_EDITOR
