#include "Header.h"

WindowModel::WindowModel()
{

}

WindowModel::~WindowModel()
{
}

void WindowModel::SetStartConfig()
{
	NotifyUpdateMode(mode_str[curr_status].c_str());
	NotifyUpdateFilename(filename.c_str());
	NotifyUpdateLineStats(num_curr_line, num_lines);
	char hello_message[] = "Use :h for help\nUse :o filename to open file";
	NotifyPrintMsg(hello_message);
}
void WindowModel::SetStatus(status new_status)
{
	curr_status = new_status;
	NotifyUpdateMode(mode_str[new_status].c_str());
}

void WindowModel::SetFilename(const char* new_filename)
{
	filename = new_filename;
	NotifyUpdateFilename(new_filename);
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
		NotifyUpdateCmd(str.c_str());
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
		NotifyUpdateCmd(str.c_str());
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
			NotifyPrintMsg(file_data.c_str());
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
	NotifyUpdateFilename(s_filename.c_str());
	CountLines();
	NotifyUpdateLineStats(num_curr_line, num_lines);
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
		m_ProcPressedDollar();
		break;
	}
	case '0': {
		m_ProcPressedZero();
		break;
	}
	case KEY_DOWN:
	{
		m_ProcPressedKeyDown();
		break;
	}
	case KEY_UP:
	{
		m_ProcPressedKeyUp();
		break;
	}
	case KEY_LEFT:
	{
		m_ProcPressedKeyLeft();
		break;
	}
	case KEY_RIGHT:
	{
		m_ProcPressedKeyRight();
		break;
	}
	case 'b':
	{
		NotifyPressedb();
		break;
	}
	case 'w': {
		ProcPressedw();
		break;
	}
	case 'g':
	{
		//NotifyPressedg();
		break;
	}
	default:
		//str.append(1, key);
		break;
	}
	return 1;
}
void WindowModel::m_GetYX(int* y, int* x)
{
	GetViewYX(y, x);
}

void WindowModel::m_ProcPressedKeyLeft()
{
	m_GetYX(&y, &x);
	if (x == 0)
		return;
	--idx; --x_nav; --x;
	NotifyMoveCursor(y, x_nav);
}
void WindowModel::m_ProcPressedKeyRight()
{
	m_GetYX(&y, &x);
	char c = file_data[idx + 1];
	if (c != '\n' && c != '\0') {
		++idx; ++x_nav; ++x;
		NotifyMoveCursor(y, x_nav);
	}
}
void WindowModel::m_ProcPressedDollar()
{
	m_GetYX(&y, &x);
	if (file_data[idx] == '\n') //idx, not idx+1 if line if \n
		return;
	while (file_data[idx] != '\n') {
		++idx;
		++x;
	}
	--idx; --x; x_nav = x;
	NotifyMoveCursor(y, x_nav);
}
void WindowModel::m_ProcPressedZero()
{
	m_GetYX(&y, &x);
	if (x == 0)
		return;
	while (x > 0) {
		--idx; --x;
	}
	x_nav = 0; 
	NotifyMoveCursor(y, x_nav);
}
size_t WindowModel::m_CountIdxFirstLineDown(int n) const
{
	int curr_idx_f = idx_first_line;
	for (int i = 0; i < n; i++)
	{
		while (file_data[curr_idx_f] != '\n')
			curr_idx_f++;
		++curr_idx_f;
	}
	return curr_idx_f;
	//idx_first_line = curr_idx_f;
}

size_t WindowModel::m_CountIdxFirstLineUp(int n) const
{
	int curr_idx_f = idx_first_line;
	for (int i = 0; i < n; i++)
	{
		while (file_data[curr_idx_f] != '\n' && curr_idx_f > 0)
			curr_idx_f--;
		--curr_idx_f;
	}
	if (curr_idx_f > 0) return curr_idx_f;
	else return 0;
}

bool WindowModel::m_ScrollDown(int curr_pos, int n)
{
	if (file_data[idx + 1] == 0) {
		beep();
		return false;
	}
	idx_first_line = m_CountIdxFirstLineDown(n);
	const char* ptr = file_data.c_str() + idx_first_line;
	NotifymvPrintMsg(ptr, 0, 0);
	return true;
}

void WindowModel::m_ProcPressedKeyDown()
{
	if (file_data[idx + 1] == '\0') {
		beep();
		return;
	}

	int curr_pos = 0, curr_idx = idx;
	m_GetYX(&y, &x);

	if (file_data[idx] == '\n') {
		idx++;
	}
	else idx = file_data.find("\n", idx) + 1;

	while (curr_pos < (x_nav + 1) && file_data[idx] != '\n' && file_data[idx] != '\0')
	{
		//(x_nav + 1) чтобы --curr_pos после цикла всегда давало верный шаг
		++curr_pos; ++idx;
	}
	--idx; --curr_pos;
	if (curr_pos == -1) {
		idx++;
		curr_pos++;
	}

	char c = file_data[idx];
	if (y + 1 > IDX_LAST_LINE) {
		if (m_ScrollDown(curr_pos, 1))
		{
			idx_last_line = idx - curr_pos;
			num_curr_line++;
			NotifyUpdateLineStats(num_curr_line, num_lines);
			NotifyMoveCursor(y, curr_pos);
		}
		else idx = curr_idx;
	}
	else {
		num_curr_line++;
		NotifyUpdateLineStats(num_curr_line, num_lines);
		NotifyMoveCursor(y + 1, curr_pos); //после этого, чтобы курсор был в текста
	}
}
bool WindowModel::m_find_compare(const char* str, size_t len, size_t pos) const
{
	for (size_t i = 1; i < len; ++i)
	{
		if (file_data[pos + i] != *(str + i))
			return false;
	}
	return true;
}
size_t WindowModel::m_reverse_find(const char* str, size_t start_idx, size_t len_s)
{
	size_t ResultPos = STD::MyString::npos;
	if (start_idx == 0)
		return ResultPos;
		//pos = _length - 1;
	if (len_s == 0)
		return 0;
	//return ResultPos;
	size_t len_ = file_data.length();
	for (; start_idx > 0; --start_idx)
	{
		if (file_data[start_idx] == *str && m_find_compare(str, len_s, start_idx))
		{
			ResultPos = start_idx;
			break;
		}
	}
	if (start_idx == 0) {
		if (file_data[start_idx] == *str && m_find_compare(str, len_s, start_idx)) {
			ResultPos = start_idx;
		}
	}
	return ResultPos;
}

size_t WindowModel::m_ReversFind(const char* str, size_t start_idx)
{
	return m_reverse_find(str, start_idx, strlen(str));
}

void WindowModel::m_ProcPressedKeyUp()
{
	m_GetYX(&y, &x);

	if (idx - x == 0) {
		beep();
		return;
	}

	int curr_pos = 0;
	size_t temp_idx = 0, next_idx = 0;

	if (file_data[idx] == '\n') {
		idx--;
	}
	else idx = m_ReversFind("\n", idx);

	if (idx != 0) {
		next_idx = m_ReversFind("\n", idx - 1);
		if (next_idx == STD::MyString::npos) {
			if (file_data[idx - 1] == '\n') {}
			else idx = 0;
		}
		else idx = next_idx + 1;
	}

	while (curr_pos < (x_nav + 1) && file_data[idx] != '\n' && file_data[idx] != '\0')
	{
		//(x_nav + 1) чтобы --curr_pos после цикла всегда давало верный шаг
		++curr_pos; ++idx;
	}
	--idx; --curr_pos;
	if (curr_pos == -1) {
		idx++;
		curr_pos++;
	}

	/*
	while (x > 0) {
		x--; idx--; //idx начала строки
	}
	if (idx != 0) {
		idx--;
		temp_idx = m_ReversFind("\n", idx);
		if (temp_idx == STD::MyString::npos)
		/*while (file_data[idx] != '\n' && idx != 0) {
			idx--;
		}*/

	/*if (file_data[idx] == '\n') {
		idx--;
		*/
		/*if (idx != 0) {
			if (file_data[idx - 1] != '\n') {
				temp_idx = m_ReversFind("\n", idx - 1);
				if (temp_idx == STD::MyString::npos) {
					//idx = 0;
				}
				else if (idx != temp_idx) idx = temp_idx + 1;
			}
		}*/
	//}
	//else {
	//	temp_idx = m_ReversFind("\n", idx);
	//	if (temp_idx == STD::MyString::npos) {
	//		beep();
	//		return;
	//	}
	//	if (temp_idx != 0) {
	//		idx = temp_idx; temp_idx = m_ReversFind("\n", idx - 1);
	//		if (temp_idx == STD::MyString::npos) {
	//			//idx = temp_idx;
	//		}
	//		else idx = temp_idx + 1;
	//	}
	//	else idx = temp_idx;
	//}
	//while (curr_pos < (x_nav + 1) && file_data[idx] != '\n')
	//{
	//	++curr_pos; ++idx;
	//}
	//--idx; --curr_pos;
	//if (curr_pos == -1) {
	//	idx++; curr_pos++;
	//}
	

	if (y - 1 < 0)
	{
		//TODO scroll
	}
	else {
		num_curr_line--;
		NotifyUpdateLineStats(num_curr_line, num_lines);
		NotifyMoveCursor(y - 1, curr_pos);
	}
}

void WindowModel::ProcPressedw()
{
	int new_x = x, new_y = y;
	size_t temp_idx = idx;

	if (file_data[idx + 1] != '\0') {
		size_t idx_ = file_data.find(" ", idx);
		size_t idx_n = file_data.find("\n", idx);

		if (idx_ < idx_n) { //_ раньше '\n'
			if (file_data[idx_ + 1] == ' ') {

			}
			else if (file_data[idx_ + 1] == '\0') {
				beep();
				return;
			}
			else if (file_data[idx_ + 1] == '\n') {

			}
			else {
				idx = idx_ + 1;
				x_nav += idx - temp_idx; x = x_nav;
				NotifyMoveCursor(y, x_nav);
			}

		}
		else if (idx_ != STD::MyString::npos) {
			new_y++;
			if (file_data[idx_n + 1] == ' ') {

			}
			else if (file_data[idx_n + 1] == '\0') {
				beep();
				return;
			}
			else if (file_data[idx_n + 1] == '\n') {

			}
			else {
				idx = idx_n + 1;
				x_nav = 0; x = x_nav;
				NotifyMoveCursor(new_y, x_nav);
			}
		}
	}
	else beep();
}