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
    wrefresh(mode_win);
}

void ConsoleView::UpdateCmd()
{
    int y, x;
    werase(cmd_win);
    waddch(cmd_win, ':');
    //wprintw(cmd_win, m_mymodel->buffer);
    wprintw(cmd_win, m_mymodel->str.c_str());
    //wmove(m_cmd_win, y, x + 1);
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

/*
void ConsoleView::clear_line_cmd()
{
    attron(COLOR_PAIR(1));
    mvhline(m_mymodel->MAX_NLINES - 1, 0, ' ', m_mymodel->MAX_NCOLS);
    if (m_mymodel->curr_status == WindowModel::COMMAND)
    {
        attron(COLOR_PAIR(1));
        addch(':');
        attroff(COLOR_PAIR(1));
        //move(m_mymodel->MAX_NLINES - 1, 1);
        refresh();
    }
    attroff(COLOR_PAIR(1));
    refresh();
}

void ConsoleView::clear_line_stat()
{
    mvhline(m_mymodel->MAX_NLINES - 2, 0, ' ', m_mymodel->MAX_NCOLS);
    refresh();
}
*/