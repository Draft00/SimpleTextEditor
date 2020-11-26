#include "Header.h"

WindowModel::WindowModel()
{

	/*initscr();
	resize_term(MAX_NLINES, MAX_NCOLS); //int nlines, int ncols
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_CYAN);
	create_cmd_line();
	create_line_stat();
	print_hello_message();

	move(0, 0);
	refresh();*/
	//m_local_win = create_newwin(MAX_NLINES, MAX_NCOLS, 0, 0);
}

WindowModel::~WindowModel()
{
}

void WindowModel::SetStartConfig()
{
	NotifySetStartConfig();
}
void WindowModel::SetStatus(status new_status)
{
	curr_status = new_status;
	NotifyUpdateMode();
}

void WindowModel::SetFilename(const char* new_filename)
{
	filename = new_filename;
	NotifyUpdateFilename();
}

int WindowModel::GetKeyFromCmd(int key)
{
	switch (key)
	{
	case 8:
	{
		if (!str.empty())
		{
			str.erase(str.length() - 1, 1);
		}
		NotifyUpdateCmd();
		break;
	}
	case 13:
	{
		NotifyClearCmd();
		int command = -1;
		command = ParseCommand();
		str.clear();
		idx = 0;
		if (command == STOP)
		{
			return -1;
		}
		break;
	}
	case 27: //ESC
	{
		str.clear();
		NotifyEndCmd();
		SetStatus(NAVIGATION);
		return 0;
	}
	default:
		str.append(1, key);
		NotifyUpdateCmd();
		break;
	}
	return 1;
}

int WindowModel::ParseCommand()
{
	if (!str.empty())
	{
		if (str == "q!")
		{
			//TODO закрыть документ
			return STOP;
		}
		if (str == "x")
		{

		}
		else if (str == "w")
		{

		}
		else
		{

		}
	}
}
int WindowModel::GetKeyFromNavigation(int key)
{
	return 0;
}
