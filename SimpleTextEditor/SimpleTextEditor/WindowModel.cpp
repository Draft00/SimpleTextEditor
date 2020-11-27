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
		if (command == STOP) {
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
		break;
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
		if (str == "q!") {
			return STOP;
		}
		else if (str == "q") {
			if (flag_changes == 1) {
				beep(); //TODO print message
			}
			return STOP;

		}
		else if (str == "x") {
			SaveFile();
			return STOP;
		}
		else if (str == "w") {
			SaveFile();
		}
		else if (str[0] == 'w') {
			STD::MyString filename_save;
			filename_save = str.substr(2);
			SaveFile(filename_save);
		}
		else if (str[0] == 'o') {
			STD::MyString filename;
			filename = str.substr(2);
			OpenFile(filename);
			NotifyPrintNewText();
			NotifyClearCmd(); //to return cursor in CMD
		}
		else if (str == "wq!") {
			SaveFile();
			return STOP;
		}
		else if (str == "number") {

		}
		else if (str == "h") {

		}
		else
		{

		}
	}
	return 1;
}
void WindowModel::SaveFile()
{
	std::ofstream fout;
	fout.open(filename.c_str(), std::ios_base::out);
	if (!fout.is_open()) {
		beep(); //TODO return message
		return;
	}
	fout << file_data;
	fout.close();
}
void WindowModel::SaveFile(STD::MyString s_filename)
{
	std::ofstream fout;
	fout.open(s_filename.c_str(), std::ios_base::out);
	if (!fout.is_open()) {
		beep(); //TODO return message
		return;
	}
	fout << file_data;
	fout.close();
}

void WindowModel::OpenFile(STD::MyString s_filename)
{
	char c;
	std::ifstream fin;
	fin.open(s_filename.c_str(), std::ios_base::in);
	if (!fin.is_open()) {
		beep(); //TODO RETURN MESSAGE
		return;
	}
	while (!fin.eof()) {
		fin.get(c);
		file_data.append(1, c);
	}
	fin.close();
	filename = s_filename;
	NotifyUpdateFilename();
}

void WindowModel::CountLines()
{
	size_t pos = 0, idx = 0;
	while (pos != -1)
	{
		pos = file_data.find("\n", idx);
		idx = pos + 1;
		num_lines++;
	}
	num_lines -=2;
}
int WindowModel::GetKeyFromNavigation(int key)
{
	switch (key)
	{
	case '$': {
		NotifyPressedDollar();
		break;
	}
	case '0': {
		NotifyPressedZero();
		break;
	}
	case 'w': {
		break;
	}
	case KEY_DOWN:
	{
		NotifyPressedKeyDown();
		break;
	}
	case KEY_UP:
	{
		NotifyPressedKeyUp();
		break;
	}
	case KEY_LEFT:
	{

	}
	case KEY_RIGHT:
	{

	}
	case 'b':
	{
		break;
	}
	case 'g':
	{
		
		break;
	}
	default:
		str.append(1, key);
		break;
	}
	return 1;
}
