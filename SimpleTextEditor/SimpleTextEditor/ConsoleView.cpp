#include "Header.h"

ConsoleView::ConsoleView(WindowModel* model)
{
    m_mymodel = model;
    m_mymodel->AddObserver(this);

    initscr();
    resize_term(MAX_NLINES, MAX_NCOLS); //int nlines, int ncols

    init_coloros_pair();

    text_win = create_text_win();
    cmd_win = create_cmd_win();
    create_status_wins();

}

ConsoleView::~ConsoleView()
{
    delwin(text_win);
    delwin(cmd_win);
    delwin(mode_win);
    delwin(filename_win);
    delwin(line_stats_win);
    endwin();
}

void ConsoleView::PressedDollar()
{
    getyx(text_win, y, x);
    if (m_mymodel->file_data[m_mymodel->idx] == '\n') //idx, not idx+1 if line if \n
        return;
    while (m_mymodel->file_data[m_mymodel->idx] != '\n')
    {
        m_mymodel->idx++;
        ++x;
    }
    m_mymodel->idx -= 1;
    x_nav = x - 1;
    wmove(text_win, y, x - 1);
    wrefresh(text_win);
}
void ConsoleView::PressedZero()
{
    getyx(text_win, y, x);
    if (x == 0)
        return;
    while (x > 0) {
        m_mymodel->idx--;
        --x;
    }
    x_nav = 0;
    wmove(text_win, y, 0);
    wrefresh(text_win);
}
void ConsoleView::PressedKeyDown()
{
    if (m_mymodel->file_data[m_mymodel->idx + 1] != '\0') {
        int curr_pos = 0;
        getyx(text_win, y, x);

        if (m_mymodel->file_data[m_mymodel->idx] == '\n') {
            m_mymodel->idx++;
        }
        else m_mymodel->idx = m_mymodel->file_data.find("\n", m_mymodel->idx) + 1;

        while (curr_pos < (x_nav + 1) && m_mymodel->file_data[m_mymodel->idx] != '\n' && m_mymodel->file_data[m_mymodel->idx] != '\0')
        {
            //(x_nav + 1) чтобы --curr_pos после цикла всегда давало верный шаг
            ++curr_pos; m_mymodel->idx++;
        }
        m_mymodel->idx--;  --curr_pos;
        if (curr_pos == -1) {
            m_mymodel->idx++;
            curr_pos++;
        }

        char c = m_mymodel->file_data[m_mymodel->idx];
        if (y + 1 > TEXT_W_LINES - 1)
        {
            wscrl(text_win, 1);
            wrefresh(text_win);
            //char c = m_mymodel->file_data[m_mymodel->idx];
            size_t idx_print = m_mymodel->idx - curr_pos;
            size_t pos = m_mymodel->file_data.find("\n", idx_print);
            if (pos == STD::MyString::npos) {
                beep();
                return;
            }
            STD::MyString str_print = m_mymodel->file_data.substr(idx_print, pos - idx_print);
            mvwprintw(text_win, y, 0, str_print.c_str());
            m_mymodel->num_curr_line++;
            UpdateLineStats();
            wmove(text_win, y, curr_pos);
        }
        else {
            m_mymodel->num_curr_line++;
            UpdateLineStats();
            wmove(text_win, y + 1, curr_pos);
        }
        wrefresh(text_win);
    }
    else {
        beep();
    }
}
void ConsoleView::PressedKeyUp()
{
    int temp_x = 0;
    getyx(text_win, y, x);
    if (m_mymodel->idx - x > 0) {
        int curr_pos = 0;
        //getyx(text_win, y, x);

        if (m_mymodel->file_data[m_mymodel->idx] == '\n') {
            m_mymodel->idx--;
            if (m_mymodel->file_data[m_mymodel->idx - 1] != '\n')
                m_mymodel->idx--;
        }
        else
        {
            while (m_mymodel->file_data[m_mymodel->idx] != '\n') {
                m_mymodel->idx--;
            }
            if (m_mymodel->idx > 0 && m_mymodel->file_data[m_mymodel->idx - 1] != '\n') {
                m_mymodel->idx--;
            }
            else {

            }
                
        }
        if (m_mymodel->file_data[m_mymodel->idx] != '\n')
        {
            while (m_mymodel->file_data[m_mymodel->idx] != '\n' && m_mymodel->idx > 0) {
                m_mymodel->idx--;
            }
            //if (m_mymodel->idx == STD::MyString::npos) m_mymodel->idx = 0;
            if (m_mymodel->idx != 0)
                m_mymodel->idx++;
        }
        while (curr_pos < (x_nav + 1) && m_mymodel->file_data[m_mymodel->idx] != '\n') { //&& m_mymodel->file_data[m_mymodel->idx] != '\0') {
            ++curr_pos; m_mymodel->idx++;
        }
        m_mymodel->idx--;  --curr_pos;
        if (curr_pos == -1) {
            m_mymodel->idx++;
            curr_pos++;
        }
        if (y - 1 < 0)
        {
            wscrl(text_win, -1);
            wrefresh(text_win);
            //char c = m_mymodel->file_data[m_mymodel->idx];
            size_t idx_print = m_mymodel->idx - curr_pos;
            size_t pos = m_mymodel->file_data.find("\n", idx_print);
            if (pos == STD::MyString::npos) {
                beep();
                return;
            }
            STD::MyString str_print = m_mymodel->file_data.substr(idx_print, pos - idx_print);
            mvwprintw(text_win, y, 0, str_print.c_str());
            m_mymodel->num_curr_line--;
            UpdateLineStats();
            wmove(text_win, y, curr_pos);
        }
        else {
            m_mymodel->num_curr_line--;
            UpdateLineStats();
            wmove(text_win, y - 1, curr_pos);
        }
        wrefresh(text_win);
    }
    else beep();
}
void ConsoleView::PressedW()
{
}
void ConsoleView::PressedB()
{

}
void ConsoleView::USetStartConfig()
{
    wprintw(mode_win, mode_str[m_mymodel->curr_status].c_str());
    wrefresh(mode_win);
    wprintw(filename_win, m_mymodel->filename.c_str());
    wrefresh(filename_win);
    wprintw(line_stats_win, "Line: %d/%d", m_mymodel->num_curr_line, m_mymodel->num_lines);
    wrefresh(line_stats_win);
    char hello_message[] = "Use :h for help\nUse :o filename to open file";
    wprintw(text_win, hello_message);
    wrefresh(text_win);
}

void ConsoleView::ClearCmd()
{
    werase(cmd_win);
    waddch(cmd_win, ':');
    wrefresh(cmd_win);
}

void ConsoleView::EndCmd()
{
    werase(cmd_win);
    wrefresh(cmd_win);
}

void ConsoleView::UpdateMode()
{
    werase(mode_win);
    wprintw(mode_win, mode_str[m_mymodel->curr_status].c_str());
    wrefresh(mode_win);
    if (m_mymodel->curr_status == WindowModel::COMMAND)
    {
        wprintw(cmd_win, ":");
        wrefresh(cmd_win);
    }
}

void ConsoleView::UpdateFilename()
{
    werase(filename_win);
    wprintw(filename_win, m_mymodel->filename.c_str());
    wrefresh(filename_win);
}

void ConsoleView::PrintNewText()
{
    wclear(text_win);
    wprintw(text_win, (m_mymodel->file_data.c_str() + m_mymodel->idx));
    wrefresh(text_win);

    m_mymodel->CountLines();
    UpdateLineStats();

    /*if (m_mymodel->curr_status == WindowModel::COMMAND)
    {
        wmove(cmd_win, 0, 1);
        wrefresh(cmd_win);
    }*/
}

void ConsoleView::UpdateLineStats()
{
    wclear(line_stats_win);
    wprintw(line_stats_win, "Line: %d/%d", m_mymodel->num_curr_line, m_mymodel->num_lines);
    wrefresh(line_stats_win);
}

void ConsoleView::UpdateCmd()
{
    werase(cmd_win);
    waddch(cmd_win, ':');
    wprintw(cmd_win, m_mymodel->str.c_str());
    wrefresh(cmd_win);
}


/*
    private methods
*/
WINDOW* ConsoleView::create_cmd_win()
{
    WINDOW* cmd_win;
    cmd_win = newwin(1, MAX_NCOLS, MAX_NLINES-1, 0);
    wbkgd(cmd_win, COLOR_PAIR(1));
    wrefresh(cmd_win);
    return cmd_win;
}

WINDOW* ConsoleView::create_text_win()
{
    WINDOW* text_win;
    text_win = newwin(TEXT_W_LINES, TEXT_W_COLS, 0, 0);
    wbkgd(text_win, COLOR_PAIR(2));
    wrefresh(text_win);
    return text_win;
}

void ConsoleView::init_coloros_pair()
{
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
}

void ConsoleView::create_status_wins()
{
    mode_win = newwin(1, 4, MAX_NLINES - 2, 0);
    wbkgd(mode_win, COLOR_PAIR(3));
    wrefresh(mode_win);
    filename_win = newwin(1, 30, MAX_NLINES - 2, 4);
    wbkgd(filename_win, COLOR_PAIR(2));
    wrefresh(filename_win);
    line_stats_win = newwin(1, MAX_NCOLS - 34, MAX_NLINES - 2, 34);
    wbkgd(line_stats_win, COLOR_PAIR(3));
    wrefresh(line_stats_win);
}