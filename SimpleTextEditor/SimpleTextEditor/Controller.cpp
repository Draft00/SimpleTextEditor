#include "Header.h"

void Controller::start()
{
    m_mymodel->SetStartConfig();

    raw();
    keypad(stdscr, true);
    int global_num = 1;
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
            } while (stop);
        }
        if (m_mymodel->curr_status == WindowModel::NAVIGATION)
        {
            noecho();
            keypad(m_view->text_win, true);
            wmove(m_view->text_win, 0, 0);
            do {
                m_choice = wgetch(m_view->text_win);
                switch (m_choice)
                {
                case KEY_UP:
                {
                    //presssed_up();
                    global_num = 0;
                    break;
                }
                case KEY_DOWN:
                {
                    pressed_down();
                    break;
                }
                case KEY_LEFT:
                {
                    pressed_left();
                    break;
                }
                case KEY_RIGHT:
                {
                    pressed_right();
                    break;
                }
                case '0':
                {
                    pressed_beg_line();
                    break;
                }
                case '$':
                {
                    pressed_end_line();
                    break;
                }
                default:
                    break;
                }
            } while (m_choice != 'Q');
            keypad(stdscr, false);
            echo();
        }
    }
}

void Controller::get_filename()
{
    char filename_s[560] = { 0 };
    int i = 0;
    int c = 0;
    nonl();
    int stop = 1;
    while (stop)
    {
        c = getch();
        switch (c)
        {
        case PADENTER:
        {
            filename_s[i] = '\0';
            stop = 0;
            break;
        }
        case 8:
        {
            //удалить символ (и закрашивать при этом).
            //i--;
        }
        default:
        {
            filename_s[i] = c;
            i++;
            break;
        }
        }
    }
    m_mymodel->SetFilename(filename_s);
}

void Controller::presssed_up()
{
	if (m_y > 0)
	{
		m_y--;
		move(m_y, m_x);
		refresh();
	}
}
void Controller::pressed_down() //TODO SKROLLING
{
    if (m_y < (m_mymodel->MAX_NLINES - 3))
    {
        m_y++;
        move(m_y, m_x);
        refresh();
    }
}
void Controller::pressed_right()
{
    if (m_x < m_mymodel->MAX_NCOLS)
    {
        m_x++;
        move(m_y, m_x);
        refresh();
    }
}

void Controller::pressed_left()
{
    if (m_x > 0)
    {
        m_x--;
        move(m_y, m_x);
        refresh();
    }
}
void Controller::pressed_beg_line()
{
    m_x = 0;
    move(m_y, m_x);
    refresh();
}

void Controller::pressed_end_line()
{
    m_x = (m_mymodel->MAX_NCOLS - 1);
    move(m_y, m_x);
    refresh();
}