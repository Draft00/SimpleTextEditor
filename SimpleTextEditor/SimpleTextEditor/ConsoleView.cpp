#include "Header.h"


/*
    private 
*/
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
