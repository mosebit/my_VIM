#include "ModelMVC.h"

// error codes
#define FOPEN_ERR -11

// consts
#define MAX_LINE_LEN 10000

Model::Model(const char* path) {
	this->file_path = path;
	set_file_name();
	read_from_file(path);
	i_j_prev.first.first = -1;
	i_j_prev.first.second = -1;
	i_j_prev.second = -1;
}
Model::~Model() {
}

void Model::model_open_new_file(const char* path) {
	file_lines.clear();
	view_lines.clear();

	this->file_path = path;
	set_file_name();
	read_from_file(path);
	i_j_prev.first.first = -1;
	i_j_prev.first.second = -1;
	i_j_prev.second = -1;
}

void Model::set_file_name() {
	int i = 0;
	for (i = this->file_path.length(); i > -1; i--) {
		if (this->file_path[i] == '\\')
			break;
	}
	this->file_name = this->file_path.c_str() + i + 1;
}
MyString Model::get_file_name() {
	return this->file_name;
}

int Model::get_num_of_file_lines() {
	return this->file_lines.size();
}

MyString Model::get_line(int y) {
	int counter = 0;
	for (int i = 0; i < view_lines.size(); i++) {
		for (int j = 0; j < view_lines[i].size(); j++) {
			if (counter == y)
				return this->view_lines[i][j];
			counter++;
		}
	}
}

int Model::get_line_len(int y) {
	return this->get_line(y).length();
}

void Model::read_from_file(MyString path) {
	char uno_line_from_file[MAX_LINE_LEN] = { 0 };
	if (ifstr_model.is_open()) ifstr_model.close();
	ifstr_model.open(path.c_str(), std::ios::in);
	if (!ifstr_model.is_open()) exit(FOPEN_ERR);
	while (ifstr_model.getline(uno_line_from_file, MAX_LINE_LEN)) {
		file_lines.push_back(uno_line_from_file);
	}
	return;
}

void Model::prepare_lines_for_view() {
	int num_lines = get_num_of_file_lines();
	int screen_width = COLS - 1;
	int str_len = 0;
	short num_copy_chars = 0;

	vector <MyString> tmp;
	this->num_of_view_lines = 0;
	view_lines.clear();
	for (int i = 0; i < num_lines; i++) {

		if (this->file_lines[i].length() > screen_width) {
			str_len = this->file_lines[i].length();
			for (int ind_copy_from = 0; ind_copy_from < str_len; ) {
				num_copy_chars = str_len - ind_copy_from > screen_width ? screen_width : str_len - ind_copy_from;
				tmp.push_back(this->file_lines[i].substr(ind_copy_from, num_copy_chars));
				ind_copy_from += num_copy_chars;
				this->num_of_view_lines++;
			}
			this->view_lines.push_back(tmp);
			tmp.clear();
		}
		else {
			this->num_of_view_lines++;
			tmp.push_back(this->file_lines[i]);
			this->view_lines.push_back(tmp);
			tmp.clear();
		}
	}
	return;
}
int Model::show_num_of_view_lines() {
	return this->num_of_view_lines;
}

void Model::update_num_of_view_lines() {
	int new_num_of_view_lines = 0;
	for (int i = 0; i < this->view_lines.size(); i++) {
		new_num_of_view_lines += this->view_lines[i].size();
	}
	this->num_of_view_lines = new_num_of_view_lines;
}


void Model::view_to_model(View* view_arg) {
	this->view_p = view_arg;
}

void Model::delete_substr(int y, int x, int num_to_del) {
	this->file_lines[y].erase(x, num_to_del);
}
void Model::connect_lines(int y_to_del, int y_to_expand) {
	this->view_p->put_curs_on_wrap(y_to_expand, 0);
	this->view_p->edit_cursor_x_pos(-1);

	this->file_lines[y_to_expand].append(file_lines[y_to_del].c_str());
	this->file_lines.erase(file_lines.begin() + y_to_del);
}


// Для ввода
void Model::split_lines() {
	int x_pos = this->view_p->get_x_pos();
	int y_pos = this->view_p->get_y_pos();

	int num_of_view_line = this->view_p->get_y_pos() + this->view_p->show_first_line_position();

	int x_model_pos = -1;	// номер буквы для отрезания строки именно в преставлении целой строки
	int y_model_pos = 0;

	// поиск номера строки и координаты x для редактирования (в строке, которая была скопирована из файла и не обрезана под ширину экрана) 
	int counter = 0;

	int i = this->view_p->find_i_j_in_view_strings(num_of_view_line).i;
	int j = this->view_p->find_i_j_in_view_strings(num_of_view_line).j;

	x_model_pos = 0;
	y_model_pos = i;
	for (int k = 0; k < j; k++) {
		x_model_pos += view_lines[i][k].length();
	}
	x_model_pos += x_pos;

	MyString second_str_part = this->file_lines[y_model_pos].c_str() + x_model_pos;
	this->file_lines[y_model_pos].erase(x_model_pos, this->file_lines[y_model_pos].length() - x_model_pos);
	this->file_lines.insert(file_lines.begin() + y_model_pos + 1, second_str_part);
	this->view_lines.clear();
	this->prepare_lines_for_view();

	if (y_pos == MAX_SHOW_LINES_COUNT - 1) {
		this->view_p->change_first_line_pos(1);
		this->view_p->put_curs_on_wrap(y_pos, 0);
	}
	else {
		this->view_p->new_update_page_content(this->view_p->show_first_line_position());
		this->view_p->put_curs_on_wrap(y_pos + 1, 0);
	}

}

// изменяет вид экранной строки (модифицирована под ширину экрана) по номеру строки в памяти (оригинальная полной длиной) 
void Model::update_view_lines(int module_str_n) {
	this->view_lines.erase(this->view_lines.begin() + module_str_n);
	vector <MyString> str_vector;

	int screen_width = COLS - 1;
	short num_copy_chars = 0;
	if (this->file_lines[module_str_n].length() > screen_width) {
		int str_len = this->file_lines[module_str_n].length();
		for (int ind_copy_from = 0; ind_copy_from < str_len; ) {
			num_copy_chars = str_len - ind_copy_from > screen_width ? screen_width : str_len - ind_copy_from;
			str_vector.push_back(this->file_lines[module_str_n].substr(ind_copy_from, num_copy_chars));
			ind_copy_from += num_copy_chars;
			this->num_of_view_lines++;
		}
		this->view_lines.insert(this->view_lines.begin() + module_str_n, str_vector); // ??? мб тут проблема с индексом куда вставляем
		str_vector.clear();
	}
	else {
		this->num_of_view_lines++;
		str_vector.push_back(this->file_lines[module_str_n]);
		this->view_lines.insert(this->view_lines.begin() + module_str_n, str_vector); // ??? мб тут проблема с индексом куда вставляем
		str_vector.clear();
	}
}

void Model::delete_str_contents_by_num_in_view(int str_n) {
	i_j_struct i_j_view;
	i_j_view = this->view_p->find_i_j_in_view_strings(str_n);
	this->file_lines[i_j_view.i].clear();
	this->update_view_lines(i_j_view.i);
	if (i_j_view.j > 0)
		this->view_p->put_curs_on_wrap(this->view_p->get_y_pos() - i_j_view.j, 0);
	else
		this->view_p->put_curs_on_wrap(this->view_p->get_y_pos(), 0);
}

void Model::delete_symbol_by_view_x_y_pos(int view_y, int cur_x) {
	i_j_struct i_j_view;
	i_j_view = this->view_p->find_i_j_in_view_strings(view_y);
	this->file_lines[i_j_view.i].erase(cur_x, 1);
	this->update_view_lines(i_j_view.i);
}

void Model::change_symbol_by_view_x_y_pos(int view_y, int cur_x, char new_symbol) {
	i_j_struct i_j_view;
	i_j_view = this->view_p->find_i_j_in_view_strings(view_y);
	//this->file_lines[i_j_view[0]] + cur_x = new_symbol;
	char const_char_symbol[2] = { new_symbol, '\0' };
	this->file_lines[i_j_view.i].replace(cur_x, 1, const_char_symbol);
	this->update_view_lines(i_j_view.i);
}

void Model::insert_symbol_by_view_x_y_pos(int view_y, int cur_x, char new_symbol) {
	i_j_struct i_j_view;
	i_j_view = this->view_p->find_i_j_in_view_strings(view_y);
	//this->file_lines[i_j_view[0]] + cur_x = new_symbol;
	char const_char_symbol[2] = { new_symbol, '\0' };
	int insert_start_ind = 0;
	for (int i = 0; i < view_lines[i_j_view.i].size() && i < i_j_view.j; i++) {
		insert_start_ind += view_lines[i_j_view.i][i].length();
	}
	insert_start_ind += cur_x;
	this->file_lines[i_j_view.i].insert(insert_start_ind, const_char_symbol, 1);
	this->update_view_lines(i_j_view.i);
}

void Model::delete_char_after(int cur_y, int cur_x) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	if (view_lines[i_j.i][i_j.j].length() < 1)
		return;
	int x_pos_in_file_lines = find_x_pos_in_file_lines(cur_y, cur_x);

	//for (int incr = 0; incr < i_j.j; incr++)
	//	x_pos_in_file_lines += view_lines[i_j.i][incr].length();
	//x_pos_in_file_lines += cur_x;

	file_lines[i_j.i].erase(x_pos_in_file_lines, 1);
	update_view_lines(i_j.i);
	i_j_struct new_i_j = view_p->find_i_j_in_view_strings(cur_y);
	if (new_i_j.j == i_j.j && cur_x >= view_lines[i_j.i][i_j.j].length() && cur_x > 0)
		cur_x--;
	else if (new_i_j.j < i_j.j) {
		cur_x = 0;
		cur_y--;
	}
	view_p->update_page_content(view_p->show_first_line_position());
	view_p->put_curs_on_wrap(cur_y, cur_x);
}

struct word_edges Model::get_word_edges(int cur_y, int cur_x) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	word_edges res_word_edges;
	int i = 0, cur_str_len = view_lines[i_j.i][i_j.j].length();
	for (i = cur_x; view_lines[i_j.i][i_j.j][i] != ' ' && i > -1; i--) {}			// search start pos
	res_word_edges.word_beg = (i == -1 ? 0 : i);
	for (i = cur_x; view_lines[i_j.i][i_j.j][i] != ' ' && i < cur_str_len; i++) {}	// search word ens pos
	res_word_edges.word_end = i;

	return res_word_edges;
}

int Model::find_x_pos_in_file_lines(int cur_y, int cur_x) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	int x_pos_in_file_lines = 0;
	for (int incr = 0; incr < i_j.j; incr++)
		x_pos_in_file_lines += view_lines[i_j.i][incr].length();
	x_pos_in_file_lines += cur_x;
	return x_pos_in_file_lines;
}
void Model::delete_word_after(int cur_y, int cur_x) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	if (view_lines[i_j.i][i_j.j].length() < 1)
		return;
	if (view_lines[i_j.i][i_j.j][cur_x] == ' ') {	// if cursor on space - delete only space
		delete_char_after(cur_y, cur_x);
		return;
	}

	word_edges word_beg_end = get_word_edges(cur_y, cur_x);

	int start_pos_in_file_lines = find_x_pos_in_file_lines(cur_y, word_beg_end.word_beg);
	file_lines[i_j.i].erase(start_pos_in_file_lines, word_beg_end.word_end - word_beg_end.word_beg);
	update_view_lines(i_j.i);

	cur_x = --word_beg_end.word_beg;
	if (cur_x < 0)
		cur_x = 0;
	view_p->update_page_content(view_p->show_first_line_position());
	view_p->put_curs_on_wrap(cur_y, cur_x);
}

void Model::copy_word(int cur_y, int cur_x) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	if (view_lines[i_j.i][i_j.j].length() < 1)
		return;
	if (view_lines[i_j.i][i_j.j][cur_x] == ' ') { // then save rest spaces to the right
		buffer.clear();
		for (int i = 0; view_lines[i_j.i][i_j.j][i] != ' ' && i < view_lines[i_j.i][i_j.j].length(); i++)
			buffer += ' ';
		return;
	}
	word_edges word_beg_end = get_word_edges(cur_y, cur_x);
	buffer = view_lines[i_j.i][i_j.j].substr(word_beg_end.word_beg, word_beg_end.word_end - word_beg_end.word_beg);
}

void Model::copy_line(int cur_y) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	if (view_lines[i_j.i][i_j.j].length() < 1)
		return;
	buffer = file_lines[i_j.i];
}

void Model::copy_and_cut_line(int cur_y) {
	i_j_struct i_j = view_p->find_i_j_in_view_strings(cur_y);
	buffer = file_lines[i_j.i];
	file_lines.erase(file_lines.begin() + i_j.i);
	prepare_lines_for_view();
	view_p->update_page_content(view_p->show_first_line_position());

	cur_y -= 1 + i_j.j;
	if (cur_y < 0) {
		if (view_p->show_first_line_position() > 0) {
			view_p->change_first_line_pos((-cur_y) > view_p->show_first_line_position() ? view_p->show_first_line_position() : cur_y);
		}
		view_p->put_curs_on_wrap(0, 0);
		view_p->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
		return;
	}
	else {
		view_p->put_curs_on_wrap(cur_y, 0);
		view_p->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
	}
}

void Model::insert_after_cursor(int cur_y) {
	int screen_width = COLS - 1, cur_x = view_p->get_x_pos();
	i_j_struct i_j_0 = view_p->find_i_j_in_view_strings(cur_y);
	int start_x_pos = view_p->get_x_pos(), start_y_pos = view_p->get_y_pos();
	if (start_x_pos >= view_lines[i_j_0.i][i_j_0.j].length() && start_x_pos > 0) {
		start_x_pos = view_lines[i_j_0.i][i_j_0.j].length() - 1;
	}
	file_lines[i_j_0.i].insert(find_x_pos_in_file_lines(cur_y, start_x_pos), buffer);
	update_view_lines(i_j_0.i);
	view_p->update_page_content(view_p->show_first_line_position());

	int move_cursor_right = buffer.size(), move_cursor_down = 0, move_screen_down = 0;
	if (cur_x + move_cursor_right > screen_width) {
		move_cursor_down = (cur_x + move_cursor_right + (screen_width - 1)) / screen_width - 1; // деление с округлением в большую сторону
		move_screen_down = MAX_SHOW_LINES_COUNT - 1 - cur_y - move_cursor_down;
		move_cursor_right = cur_x + move_cursor_right - move_cursor_down * screen_width;
		if (move_screen_down < 0) {
			view_p->change_first_line_pos(-move_screen_down);
			view_p->put_curs_on_wrap(MAX_SHOW_LINES_COUNT - 1, move_cursor_right);
		}
		else {
			view_p->put_curs_on_wrap(cur_y + move_cursor_down, move_cursor_right);
		}
	}
	else {
		view_p->put_curs_on_wrap(cur_y, cur_x + move_cursor_right);
	}
}

pair<int, int> Model::search_j_x(int i, int file_x) {
	pair<int, int> j_x;
	for (int j = 0; j < view_lines[i].size(); j++) {
		if (file_x < view_lines[i][j].length()) {
			j_x.first = j;
			j_x.second = file_x;
			return j_x;
		}
		file_x -= view_lines[i][j].length();
	}
}
void Model::prev_find_coordinates(int i, int j, int x) {

}

void Model::save_coordinates_put_cursor(pair<int, int> new_j_x, int i, int res) {
	new_j_x = search_j_x(i, res);
	i_j_prev.first.first = i;
	i_j_prev.first.first = new_j_x.first;
	i_j_prev.second = new_j_x.second;
	view_p->simple_cursor_set(i, new_j_x.first, new_j_x.second);
	return;
}

void Model::find_next(int y_pos, int x_pos, MyString searching_pair) {
	i_j_struct i_j_start = view_p->find_i_j_in_view_strings(y_pos);
	int x_file = find_x_pos_in_file_lines(y_pos, x_pos);
	// поиск в первой строке вне цикла, чтобы начать поиск с определенной иксовой координаты
	int res = -1;
	if (file_lines[i_j_start.i].length() > 0)
		res = file_lines[i_j_start.i].find(searching_pair.c_str(), x_file);
	pair<int, int> new_j_x;
	// searching in first str
	if (res > -1) {
		save_coordinates_put_cursor(new_j_x, i_j_start.i, res);
		return;
	}
	// searching in next strs
	for (int strnum = ++i_j_start.i; strnum < file_lines.size(); strnum++) {
		if (file_lines[strnum].length() < 1)
			continue;
		res = file_lines[strnum].find(searching_pair.c_str(), 0);
		if (res > -1) {
			save_coordinates_put_cursor(new_j_x, strnum, res);
			return;
		}
	}
	// searching in strs from 0 to start_x_pos
	for (int strnum = 0; strnum < i_j_start.i; strnum++) {
		if (file_lines[strnum].length() < 1)
			continue;
		res = file_lines[strnum].find(searching_pair.c_str(), 0);
		if (res > -1) {
			save_coordinates_put_cursor(new_j_x, strnum, res);
			return;
		}
	}
}

void Model::find_previous(int y_pos, int x_pos, MyString searching_pair) {
	i_j_struct i_j_start = view_p->find_i_j_in_view_strings(y_pos);
	int x_file = find_x_pos_in_file_lines(y_pos, x_pos);
	// поиск в первой строке вне цикла, чтобы начать поиск с определенной иксовой координаты
	int res = -1;
	if (file_lines[i_j_start.i].length() >= searching_pair.length())
		res = file_lines[i_j_start.i].find(searching_pair.c_str(), x_file);
	pair<int, int> new_j_x;
	// searching in first str
	if (res > -1) {
		save_coordinates_put_cursor(new_j_x, i_j_start.i, res);
		return;
	}
	// searching in prev strs
	for (int strnum = --i_j_start.i; strnum > -1; strnum--) {
		if (file_lines[strnum].length() >= searching_pair.length())
			res = file_lines[strnum].find(searching_pair.c_str(), 0);
		if (res > -1) {
			save_coordinates_put_cursor(new_j_x, strnum, res);
			return;
		}
	}
	// searching from end to start x
	for (int strnum = view_lines.size() - 1; strnum > i_j_start.i; strnum--) {
		if (file_lines[strnum].length() >= searching_pair.length())
			res = file_lines[strnum].find(searching_pair.c_str(), 0);
		if (res > -1) {
			save_coordinates_put_cursor(new_j_x, strnum, res);
			return;
		}
	}
}

void Model::write_to_file(const char* file_name) {
	MyString file_to_open;
	if (file_name == NULL)
		file_to_open = file_path;
	else
		file_to_open = file_name;

	std::ofstream out;				// поток для записи
	out.open(file_to_open.c_str());	// окрываем файл для записи
	if (out.is_open())
	{
		for (int i = 0; i < file_lines.size(); i++) {
			out << file_lines[i] << std::endl;
		}
	}
}

void Model::close_file() {
	if (ifstr_model.is_open())
		ifstr_model.close();
	exit(1);
}