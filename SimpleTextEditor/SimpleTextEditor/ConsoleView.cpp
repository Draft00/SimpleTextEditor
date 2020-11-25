#include "Header.h"

ConsoleView::ConsoleView(WindowModel* model)
{
    m_mymodel = model;
    m_mymodel->AddObserver(this);

    initscr();
    resize_term(MAX_NLINES, MAX_NCOLS); //int nlines, int ncols

    init_coloros_pair();

    m_text_win = create_text_win();
    m_cmd_win = create_cmd_win();
    create_status_wins();

}

ConsoleView::~ConsoleView()
{
    delwin(m_text_win);
    delwin(m_cmd_win);
    delwin(m_mode_win);
    delwin(m_filename_win);
    delwin(m_line_stats_win);
    endwin();
}

void ConsoleView::USetStartConfig()
{
    wprintw(m_mode_win, mode_str[m_mymodel->curr_status].c_str());
    wrefresh(m_mode_win);
    wprintw(m_filename_win, m_mymodel->filename.c_str());
    wrefresh(m_filename_win);
    wprintw(m_line_stats_win, "Line: %d/%d", m_mymodel->num_curr_line, m_mymodel->num_lines);
    wrefresh(m_line_stats_win);
    char hello_message[] = "Use :h for help\nUse :o filename to open file";
    wprintw(m_text_win, hello_message);
    wrefresh(m_text_win);
}

void ConsoleView::UpdateMode()
{
    werase(m_mode_win);
    wprintw(m_mode_win, mode_str[m_mymodel->curr_status].c_str());
    wrefresh(m_mode_win);
    if (m_mymodel->curr_status == WindowModel::COMMAND)
    {
        wprintw(m_cmd_win, ":");
        wrefresh(m_cmd_win);
    }
}

void ConsoleView::UpdateFilename()
{
    werase(m_filename_win);
    wprintw(m_filename_win, m_mymodel->filename.c_str());
    wrefresh(m_mode_win);
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
    m_mode_win = newwin(1, 4, MAX_NLINES - 2, 0);
    wbkgd(m_mode_win, COLOR_PAIR(3));
    wrefresh(m_mode_win);
    m_filename_win = newwin(1, 30, MAX_NLINES - 2, 4);
    wbkgd(m_filename_win, COLOR_PAIR(2));
    wrefresh(m_filename_win);
    m_line_stats_win = newwin(1, MAX_NCOLS - 34, MAX_NLINES - 2, 34);
    wbkgd(m_line_stats_win, COLOR_PAIR(3));
    wrefresh(m_line_stats_win);
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