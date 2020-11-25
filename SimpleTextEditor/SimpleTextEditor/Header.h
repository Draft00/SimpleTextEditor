#ifndef SIMPLE_TEXT_EDITOR
#define SIMPLE_TEXT_EDITOR

#include <vector>
#include <iostream>
#include "curses.h"

class Observer
{
public:
    virtual void UpdateMode() = 0;
    virtual void UpdateCmd() = 0;
    virtual void USetStartConfig() = 0;
    virtual void UpdateFilename() = 0;
};

class Observable
{
public:
    void AddObserver(Observer* observer);
    void NotifySetStartConfig();
    void NotifyUpdateMode();
    void NotifyUpdateFilename();

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
        WAITING
    };
    status curr_status = WAITING;
    std::string filename = "none";
    int num_curr_line = 0;
    int num_lines = 0;

    void SetStatus(status new_status);
    void SetFilename(const char* new_filename);
    void get_filename();
    void create_line_stat();

    void SetStartConfig();

    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;
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
    virtual void UpdateCmd() {}
    void USetStartConfig() override;
    void UpdateFilename() override;

    /*virtual void update()
    {
        system("cls");
        printf("Temperature in Celsius: %.2f\n", _model->getC());
        printf("Temperature in Farenheit: %.2f\n", _model->getF());
        printf("Input temperature in Celsius: ");
    }*/
private:
    WindowModel* m_mymodel;
    WINDOW* m_text_win = nullptr;
    WINDOW* m_cmd_win = nullptr; 
    WINDOW* m_mode_win = nullptr;
    WINDOW* m_filename_win = nullptr;
    WINDOW* m_line_stats_win = nullptr; 

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
    Controller(WindowModel* model)
    {
        m_mymodel = model;
    }
    void start();

private:
    WindowModel* m_mymodel;

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
