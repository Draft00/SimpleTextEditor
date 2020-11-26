#include "Header.h"

void Controller::start()
{
    m_mymodel->SetStartConfig();

    raw();
    keypad(stdscr, true);
    int global_num = 1;

    m_mymodel->OpenFile("test file.txt");
    m_mymodel->NotifyPrintNewText();
    m_mymodel->curr_status = WindowModel::NAVIGATION;

    while (global_num)
    {
        if (m_mymodel->curr_status == WindowModel::WAITING)
        {
            keypad(m_view->text_win, true);
            noecho();
            do {
                m_choice = wgetch(m_view->text_win);
                switch (m_choice)
                {
                case ':':
                {
                    m_mymodel->SetStatus(WindowModel::COMMAND);
                    break;
                }
                default:
                    break;
                }
            } while (m_choice != ':');
        }
        if (m_mymodel->curr_status == WindowModel::COMMAND)
        {
            noecho();
            keypad(m_view->cmd_win, true);
            wmove(m_view->cmd_win, 0, 1);
            wrefresh(m_view->cmd_win);
            int stop = 1;
            do {
                m_choice = wgetch(m_view->cmd_win);
                stop = m_mymodel->GetKeyFromCmd(m_choice);  
            } while (stop && stop != -1);
            if (stop == -1) {
                global_num = 0;
            }
        }
        if (m_mymodel->curr_status == WindowModel::NAVIGATION)
        {
            noecho();
            keypad(m_view->text_win, true);
            scrollok(m_view->text_win, true);
            wmove(m_view->text_win, 0, 0);
            wrefresh(m_view->text_win);
            int stop = 1;
            do {
                m_choice = wgetch(m_view->text_win);
                stop = m_mymodel->GetKeyFromNavigation(m_choice);
            } while (stop);
            scrollok(m_view->text_win, false);
        }
    }
}