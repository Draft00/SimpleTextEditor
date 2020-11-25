#ifndef SIMPLE_TEXT_EDITOR
#define SIMPLE_TEXT_EDITOR

#include <vector>
#include <iostream>
#include "curses.h"

class Observer
{
public:
    virtual void update_line_stat() = 0;
    virtual void update_cmd() = 0;
};

class Observable
{
public:
    void addObserver(Observer* observer)
    {
        _observers.push_back(observer);
    }
    void notifyUpdate_line_stat()
    {
        int size = _observers.size();
        for (int i = 0; i < size; i++)
        {
            _observers[i]->update_line_stat();
        }
    }
    void notifyUpdate_cmd_line()
    {
        int size = _observers.size();
        for (int i = 0; i < size; i++)
        {
            _observers[i]->update_cmd();
        }

    }
private:
    std::vector<Observer*> _observers;
};

class WindowModel : public Observable
{
public:
    WindowModel();
    ~WindowModel();


    float getF()
    {
        return _temperatureF;
    }
    float getC()
    {
        return (_temperatureF - 32.0) * 5.0 / 9.0;
    }


    enum status {
        NORMAL = 0,
        COMMAND,
        NAVIGATION,
        SEARCH,
        WAITING
    };
    status curr_status = WAITING;
    int MAX_NLINES = 30;
    int MAX_NCOLS = 100;

    void set_status(status new_status);
    void get_filename();
    void create_line_stat();

private:
    float _temperatureF;
    WINDOW* m_local_win = NULL;
    int m_num_curr_line = 0;
    int m_num_lines = 0;
    std::string mode[5] = { "NRM", "CMD", "NAV", "SRC", "..." };
    std::string m_filename = "none";

    WINDOW* create_newwin(int height, int width, int starty, int startx);
    void destroy_win(WINDOW* local_win);
    void create_cmd_line();
    void print_hello_message();
};

class ConsoleView : public Observer
{
public:
    ConsoleView(WindowModel* model)
    {
        m_mymodel = model;
        m_mymodel->addObserver(this);
    }
    virtual void update_line_stat()
    {
        clear_line_stat();
        m_mymodel->create_line_stat();
    }
    virtual void update_cmd()
    {
        clear_line_cmd();
    }

    /*virtual void update()
    {
        system("cls");
        printf("Temperature in Celsius: %.2f\n", _model->getC());
        printf("Temperature in Farenheit: %.2f\n", _model->getF());
        printf("Input temperature in Celsius: ");
    }*/
private:
    WindowModel* m_mymodel;

    void clear_line_cmd();
    void clear_line_stat();
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
};
#endif // !SIMPLE_TEXT_EDITOR
