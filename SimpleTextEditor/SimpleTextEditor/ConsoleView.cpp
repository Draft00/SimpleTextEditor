#include "Header.h"

ConsoleView::ConsoleView(WindowModel* model)
{
    m_mymodel = model;
    m_mymodel->AddObserver(this);

    initscr();
    resize_term(MAX_NLINES, MAX_NCOLS); //int nlines, int ncols

    init_coloros_pair();

    text_win = create_text_win();
    help_win = create_text_win();

    help_pannel = new_panel(help_win);
    text_pannel = new_panel(text_win);
    set_panel_userptr(text_pannel, help_pannel);
    set_panel_userptr(help_pannel, text_pannel);
    update_panels();

    cmd_win = create_cmd_win();
    create_status_wins();

}

ConsoleView::~ConsoleView()
{
    del_panel(text_pannel);
    del_panel(help_pannel);
    delwin(help_win);
    delwin(text_win);
    delwin(cmd_win);
    delwin(mode_win);
    delwin(filename_win);
    delwin(line_stats_win);
    endwin();
}
void ConsoleView::PutYX(int* y, int* x)
{
    int y_v = 0, x_v = 0;
    getyx(text_win, y_v, x_v);
    (*y) = y_v; (*x) = x_v;
}


void ConsoleView::find_new_x_pos(int* curr_pos)
{
    while (*curr_pos < (x_nav + 1) && m_mymodel->file_data[m_mymodel->idx] != '\n' && m_mymodel->file_data[m_mymodel->idx] != '\0') {
        (*curr_pos)++; m_mymodel->idx++;
    }
    m_mymodel->idx--;  (*curr_pos)--;
    if (*curr_pos == -1) {
        m_mymodel->idx++;
        (*curr_pos)++;
    }
}
void ConsoleView::DoScroll(int n)
{
    wscrl(text_win, n);
    wrefresh(text_win);
}

void ConsoleView::scroll_down(int curr_pos)
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
    //ЭТО ВЕРНО НО ПЕРЕДЕЛЫВАТЬ
    //UpdateLineStats();
    wmove(text_win, y, curr_pos);
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
            //UpdateLineStats(); верно
            wmove(text_win, y, curr_pos);
        }
        else {
            m_mymodel->num_curr_line--;
            //UpdateLineStats(); верно
            wmove(text_win, y - 1, curr_pos);
        }
        wrefresh(text_win);
    }
    else beep();
}

void ConsoleView::MoveInText(int new_y, int new_x)
{
    wmove(text_win, new_y, new_x);
    wrefresh(text_win);
}

void ConsoleView::Pressedw()
{
    getyx(text_win, y, x);
    if (m_mymodel->file_data[m_mymodel->idx + 1] != '\0') {
        size_t temp_idx = m_mymodel->idx;

        size_t idx_ = m_mymodel->file_data.find(" ", m_mymodel->idx);
        size_t idx_n = m_mymodel->file_data.find("\n", m_mymodel->idx);
        if (idx_ < idx_n && idx_ != STD::MyString::npos) {
            if (m_mymodel->file_data[m_mymodel->idx + 1] == ' ') {

            }
            else if (m_mymodel->file_data[m_mymodel->idx + 1] == '\0') {
                beep();
                return;
            }
            else {
                m_mymodel->idx = idx_ + 1;
                x_nav += m_mymodel->idx - temp_idx;
                wmove(text_win, y, x_nav);
            }

        }
        else if (idx_ != STD::MyString::npos)
        {

        }
        /*while (m_mymodel->file_data[m_mymodel->idx] != '\n' && m_mymodel->file_data[m_mymodel->idx] != ' ') {
            m_mymodel->idx++;
        }

        if (m_mymodel->file_data[m_mymodel->idx] == ' ') {
            m_mymodel->idx++;
            while (m_mymodel->file_data[m_mymodel->idx] == ' ') m_mymodel->idx++;
            if (m_mymodel->file_data[m_mymodel->idx] == '\n') {
                m_mymodel->idx++;
                if (y + 2 > TEXT_W_LINES - 1) {
                    scroll_down(0);
                    scroll_down(0);
                    Pressedw();
                }
                else {
                    if (m_mymodel->file_data[m_mymodel->idx] == ' ') Pressedw();
                    x_nav = 0;
                    wmove(text_win, y + 2, x_nav);
                }
            }
            else {
                x_nav += m_mymodel->idx - temp_idx;
                wmove(text_win, y, x_nav);
            }
        }
        else if (m_mymodel->file_data[m_mymodel->idx] == '\n') {
            m_mymodel->idx++;
            if (y + 1 > TEXT_W_LINES - 1) {
                scroll_down(0);
                Pressedw();
            }
            else {
                if (m_mymodel->file_data[m_mymodel->idx] == ' ') {
                    m_mymodel->idx++;
                    while (m_mymodel->file_data[m_mymodel->idx] == ' ') {
                        m_mymodel->idx++;
                    }
                    if (m_mymodel->file_data[m_mymodel->idx] == '\n') {
                        m_mymodel->idx++;
                        if (y + 2 > TEXT_W_LINES - 1) {
                            scroll_down(0);
                            scroll_down(0);
                            Pressedw();
                        }
                    }
                }
                else {
                    x_nav = 0;
                    wmove(text_win, y + 1, x_nav);
                }
            }
        }*/
        wrefresh(text_win);
    }
    else beep();
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

void ConsoleView::UpdateMode(const char* str)
{
    werase(mode_win);
    wprintw(mode_win, str);
    wrefresh(mode_win);
}

void ConsoleView::UpdateFilename(const char* filename)
{
    werase(filename_win);
    wprintw(filename_win, filename);
    wrefresh(filename_win);
}

void ConsoleView::PrintMessage(const char* str)
{
    wclear(text_win);
    wprintw(text_win, str);
    wrefresh(text_win);
}
void ConsoleView::mvPrintMessage(const char* str, int y, int x)
{
    //int curr_x, curr_y;
    //getyx(text_win, curr_y, curr_x);
    //wclear(text_win);
    //move(y, x);
    //wprintw(text_win, str);
    //move(curr_y, curr_x);
    mvwprintw(text_win, y, x, str); //работает совсем не как нужно!
    wrefresh(text_win);
}

void ConsoleView::UpdateLineStats(size_t curr_line, size_t lines)
{
    wclear(line_stats_win);
    wprintw(line_stats_win, "Line: %ld/%ld", curr_line, lines);
    wrefresh(line_stats_win);
}

void ConsoleView::UpdateCmd(const char* str)
{
    werase(cmd_win);
    waddch(cmd_win, ':');
    wprintw(cmd_win, str);
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