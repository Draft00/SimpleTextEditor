#include "Header.h"

void Controller::start()
{
    m_mymodel->SetStartConfig();

    raw();
    keypad(stdscr, true);
    int global_num = 1;

    m_mymodel->OpenFile("test file.txt");
    m_mymodel->NotifyPrintMsg(m_mymodel->file_data.c_str());
    m_mymodel->curr_status = NAVIGATION;

    //ÍÅ ÓÄÀËßÒÜ 
    /*wmove(m_view->text_win, 14, 0);
    wrefresh(m_view->text_win);
    //show_panel(m_view->text_test_p);
    wprintw(m_view->help_win, "kfkfkf");
    show_panel(m_view->help_pannel);
    update_panels();
    wrefresh(m_view->help_win);
    //show_panel(m_view->text_test_p);
    hide_panel(m_view->help_pannel);
    wrefresh(m_view->help_win);
    show_panel(m_view->text_pannel);
    update_panels();
    wrefresh(m_view->text_win);
    m_view->EndCmd();
    m_view->UpdateFilename();*/

    while (global_num)
    {
        if (m_mymodel->curr_status == WAITING)
        {
            keypad(m_view->text_win, true);
            noecho();
            do {
                m_choice = wgetch(m_view->text_win);
                switch (m_choice)
                {
                case ':':
                {
                    m_mymodel->SetStatus(COMMAND);
                    m_mymodel->NotifyClearCmd();
                    break;
                }
                default:
                    break;
                }
            } while (m_choice != ':');
        }
        if (m_mymodel->curr_status == COMMAND)
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
        if (m_mymodel->curr_status == NAVIGATION)
        {
            noecho();
            keypad(m_view->text_win, true);
            //scrollok(m_view->text_win, true);
            wmove(m_view->text_win, 0, 0);
            wrefresh(m_view->text_win);
            int stop = 1;
            do {
                m_choice = wgetch(m_view->text_win);
                stop = m_mymodel->GetKeyFromNavigation(m_choice);
            } while (stop);
            //scrollok(m_view->text_win, false);
        }
    }
}