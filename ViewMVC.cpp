#include "ViewMVC.h"
#include "ControllerMVC.h"

View::View(Model* model_p) {
	this->model_pointer = model_p;
	this->model_pointer->view_to_model(this);

	//x_position = 0;
	//y_position = 0;
	first_line_position = 0;
	window_adapt = new AdapterCurses;
	//txt_to_window();
}
View::~View() {
}


//void View::txt_to_window() {
//	int lines_num = this->model_pointer->get_num_of_file_lines();
//	for (int i = 0; i < lines_num && i < MAX_SHOW_LINES_COUNT; i++) {
//		this->window_adapt->str_add(this->model_pointer->get_line(i).c_str(), i, 0);
//	}
//	this->window_adapt->put_cursor_on(MAX_SHOW_LINES_COUNT - 1, 0);
//	this->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
//	return;
//}
void View::txt_to_window() {
	int view_lines_num = this->model_pointer->show_num_of_view_lines();
	int printed_lines_counter = 0;
	for (int i = 0; printed_lines_counter < view_lines_num && printed_lines_counter < MAX_SHOW_LINES_COUNT; i++) {
		for (int j = 0; j < this->model_pointer->view_lines[i].size(); j++, printed_lines_counter++) {
			this->window_adapt->str_add(this->model_pointer->view_lines[i][j].c_str(), printed_lines_counter, 0);
		}
	}
	this->window_adapt->put_cursor_on(MAX_SHOW_LINES_COUNT - 1, 0);
	this->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
	return;
}

void View::update_page_content(int start_line) {
	for (int i = 0, j = start_line; i < MAX_SHOW_LINES_COUNT; i++, j++) {
		this->window_adapt->del_line(i);
		this->window_adapt->str_add(this->model_pointer->get_line(j).c_str(), i, 0);
	}
	this->window_adapt->put_cursor_on(MAX_SHOW_LINES_COUNT - 1, 0);
	this->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
}
void View::change_first_line_pos(int size_of_changes) {
	first_line_position += size_of_changes;
	update_page_content(first_line_position);
}

void View::new_update_page_content(int start_line) {
	int view_str_num = 0;
	int vector_ind = 0;
	int ind_inside_vector = 0;

	i_j_struct i_j_view;
	i_j_view = this->find_i_j_in_view_strings(start_line);
	vector_ind = i_j_view.i;
	ind_inside_vector = i_j_view.j;

	int lines_out = 0;
	for (int vector_num = vector_ind; lines_out < MAX_SHOW_LINES_COUNT && vector_num < model_pointer->view_lines.size(); vector_num++) {
		for (int j = 0; j < this->model_pointer->view_lines[vector_num].size() && lines_out < MAX_SHOW_LINES_COUNT; j++) {

			this->window_adapt->del_line(lines_out);
			this->window_adapt->str_add(this->model_pointer->view_lines[vector_num][j].c_str(), lines_out, 0);

			lines_out++;
		}

	}
}
void View::clear_whole_screen() {
	for(int i = 0; i < MAX_SHOW_LINES_COUNT; i++)
		this->window_adapt->del_line(i);
}
void View::statbar_update(MyString* nav_str, int y) {
	int start_x = this->get_x_pos();
	int start_y = this->get_y_pos();
	this->window_adapt->del_line(y);
	this->window_adapt->str_add(nav_str->c_str(), y, 0);
	this->window_adapt->put_cursor_on(start_y, start_x);	// возврат курсора на позицию до работы с stat_bar
}

int View::get_y_pos() {
	return this->window_adapt->get_cur_y_pos();
}
int View::get_x_pos() {
	return this->window_adapt->get_cur_x_pos();
}
chtype View::get_pressed_key() {
	return this->window_adapt->pressed_key();
}
AdapterCurses* View::show_adapter() {
	return this->window_adapt;
}


struct i_j_struct View::find_i_j_in_view_strings(int view_str_num) {
	i_j_struct my_i_j;
	int counter = 0;
	for (int i = 0; i < this->model_pointer->view_lines.size(); i++) {
		for (int j = 0; j < this->model_pointer->view_lines[i].size(); j++) {
			if (counter == view_str_num) {
				my_i_j.i = i;
				my_i_j.j = j;
				return my_i_j;
			}
			counter++;
		}
	}
}

void View::edit_cursor_x_pos(int new_x_pos) {
	int cur_y_pos = this->get_y_pos();
	if (cur_y_pos > model_pointer->show_num_of_view_lines())
		cur_y_pos = model_pointer->show_num_of_view_lines() - 1;
	int line_len = this->model_pointer->get_line_len(cur_y_pos + this->first_line_position);
	if (new_x_pos > line_len || new_x_pos == MOVE_CURSOR_ON_LAST_STR_POS) {
		new_x_pos = line_len;
		this->window_adapt->put_cursor_on(cur_y_pos, new_x_pos);
	}
}

void View::moving_cursor_or_page(chtype mode) {
	int x_position = this->get_x_pos();
	int y_position = this->get_y_pos();

	switch (mode)
	{
	case K_LEFT:
		if (x_position > 0) {
			x_position--;
			this->window_adapt->put_cursor_on(y_position, x_position);
		}
		else if (x_position == 0) {
			if (y_position > 0) {
				y_position--;
				this->window_adapt->put_cursor_on(y_position, x_position);
				this->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
			}
			else if (y_position < 1 && this->first_line_position > 0) {
				//y_position--;
				change_first_line_pos(-1);
				this->window_adapt->put_cursor_on(y_position, x_position);
				this->edit_cursor_x_pos(MOVE_CURSOR_ON_LAST_STR_POS);
			}
		}
		break;
	case K_RIGHT:
		if (x_position + 1 < this->model_pointer->get_line_len(y_position + first_line_position)) {
			int tmp1 = this->model_pointer->get_line_len(y_position + first_line_position);
			x_position++;
			this->window_adapt->put_cursor_on(y_position, x_position);
		}
		else if (x_position + 1 >= this->model_pointer->get_line_len(y_position + first_line_position)) {
			if (y_position + 1 < MAX_SHOW_LINES_COUNT) {
				y_position++;
				this->window_adapt->put_cursor_on(y_position, 0);
				//this->edit_cursor_x_pos(0);
			}
			else if (y_position + 1 >= MAX_SHOW_LINES_COUNT &&
				this->first_line_position < this->model_pointer->show_num_of_view_lines() - MAX_SHOW_LINES_COUNT) {
				y_position = MAX_SHOW_LINES_COUNT;
				change_first_line_pos(1);
				//y_position++;
				this->window_adapt->put_cursor_on(y_position - 1, 0);
				//this->edit_cursor_x_pos(0);
			}
		}
		break;

	case K_UP:
		if (y_position > 0 /*&& y_position != first_line_position*/) {
			y_position--;
			this->window_adapt->put_cursor_on(y_position, x_position);
			this->edit_cursor_x_pos(x_position);
		}
		else if (y_position == 0 && this->first_line_position != 0) {
			change_first_line_pos(-1);
			//y_position--;
			this->window_adapt->put_cursor_on(y_position, x_position);
			this->edit_cursor_x_pos(x_position);
		}
		break;
	case K_DOWN:
		if (y_position + 1 < this->model_pointer->show_num_of_view_lines()) {
			if (y_position + 1 < MAX_SHOW_LINES_COUNT) {
				y_position++;
				this->window_adapt->put_cursor_on(y_position, x_position);
				this->edit_cursor_x_pos(x_position);
			}
			else if (y_position + 1 >= MAX_SHOW_LINES_COUNT &&
				first_line_position + MAX_SHOW_LINES_COUNT < this->model_pointer->show_num_of_view_lines()) {
				y_position = MAX_SHOW_LINES_COUNT;
				change_first_line_pos(1);
				//y_position--;
				this->window_adapt->put_cursor_on(y_position - 1, x_position);
				this->edit_cursor_x_pos(x_position);
			}
		}
		break;

	case K_PAGE_UP:
		if (this->first_line_position > 0) {
			int num_of_changing = 0;
			this->first_line_position > MAX_SHOW_LINES_COUNT ? num_of_changing = -MAX_SHOW_LINES_COUNT : num_of_changing = -this->first_line_position;
			this->change_first_line_pos(num_of_changing);
			this->window_adapt->put_cursor_on(y_position, x_position);
			this->edit_cursor_x_pos(x_position);
		}
		break;
	case K_PAGE_DOWN:
		if (this->first_line_position + MAX_SHOW_LINES_COUNT < this->model_pointer->get_num_of_file_lines()) {
			int num_of_changing = 0;
			this->first_line_position + MAX_SHOW_LINES_COUNT * 2 < this->model_pointer->get_num_of_file_lines() ? num_of_changing = MAX_SHOW_LINES_COUNT : num_of_changing = (this->model_pointer->get_num_of_file_lines() - MAX_SHOW_LINES_COUNT) - this->first_line_position;
			this->change_first_line_pos(num_of_changing);
			this->window_adapt->put_cursor_on(y_position, x_position);
			this->edit_cursor_x_pos(x_position);
		}
		break;
	case 'g': {
		chtype press_res = this->window_adapt->pressed_key();
		if (press_res == 'g') {
			this->change_first_line_pos(0 - this->first_line_position);
			this->window_adapt->put_cursor_on(0, 0);
		}
		break;
	}
	case 'G': {
		if (this->model_pointer->show_num_of_view_lines() > MAX_SHOW_LINES_COUNT) {
			this->change_first_line_pos(this->model_pointer->show_num_of_view_lines() - MAX_SHOW_LINES_COUNT - this->first_line_position);
		}
		this->window_adapt->put_cursor_on(0, 0);
		break;
	}
	case '^': {
		this->window_adapt->put_cursor_on(this->get_y_pos(), 0);
		break;
	}
	case '$': {
		i_j_struct i_j_arr;
		i_j_arr = this->find_i_j_in_view_strings(this->get_y_pos());
		this->window_adapt->put_cursor_on(this->get_y_pos(), this->model_pointer->view_lines[i_j_arr.i][i_j_arr.j].length());
		break;
	}
	case 'w':
	case 'b':
		search_pos_to_move(mode);
		break;

	default:
		break;
	}
}

void View::search_pos_to_move(char left_right_b_w) {
	int beg_x = this->get_x_pos();
	int beg_y = this->get_y_pos();

	i_j_struct i_j_main;
	i_j_main = find_i_j_in_view_strings(first_line_position + beg_y);

	if (left_right_b_w == 'b') {	// left
		if (beg_x >= this->model_pointer->view_lines[i_j_main.i][i_j_main.j].length() && beg_x > 0)
			beg_x--;

		// in that case make searching on previous str
		if (beg_x == 0) {
			if (beg_y == 0 && first_line_position == 0)
				return;
			beg_y--;

			if (beg_y < 0) {
				this->change_first_line_pos(-1);
				beg_y++;
			}
			i_j_struct i_j_tmp = find_i_j_in_view_strings(first_line_position + beg_y);
			beg_x = this->model_pointer->view_lines[i_j_tmp.i][i_j_tmp.j].length() - 1;

			// if previous str is empty
			if (beg_x < 1) {
				put_curs_on_wrap(beg_y, 0);
				return;
			}

			i_j_main.i = i_j_tmp.i;
			i_j_main.j = i_j_tmp.j;
		}

		// symbol searching
		int i = 0;
		if (beg_x > 0 && model_pointer->view_lines[i_j_main.i][i_j_main.j][beg_x] != ' ' && model_pointer->view_lines[i_j_main.i][i_j_main.j][beg_x - 1] == ' ')
			beg_x--;

		for (i = beg_x; i > -1; i--) {
			if (model_pointer->view_lines[i_j_main.i][i_j_main.j][i] != ' ')
				break;
		}
		if (i == 0) {
			put_curs_on_wrap(beg_y, 0);
			return;
		}

		// space searching
		for (i; i > -1; i--) {
			if (model_pointer->view_lines[i_j_main.i][i_j_main.j][i] == ' ')
				break;
		}

		if (i == 0) {
			put_curs_on_wrap(beg_y, 0);
			return;
		}
		else {	// put cursor on symbol, not on space
			i++;
			put_curs_on_wrap(beg_y, i);
			return;
		}
	}
	else if (left_right_b_w == 'w') {	// right
		if (beg_x >= this->model_pointer->view_lines[i_j_main.i][i_j_main.j].length()) {	// symbol on last str pos
			if (beg_y >= MAX_SHOW_LINES_COUNT - 1 && this->first_line_position + MAX_SHOW_LINES_COUNT >= this->model_pointer->view_lines.size()) {
				return;
			}
			else if (beg_y >= MAX_SHOW_LINES_COUNT - 1) {
				this->change_first_line_pos(1);
				put_curs_on_wrap(beg_y, 0);
				return;
			}

			put_curs_on_wrap(++beg_y, 0);
			return;
		}
		// space searching
		int iter = 0, cur_str_len = this->model_pointer->view_lines[i_j_main.i][i_j_main.j].length();
		for (iter = beg_x; iter < cur_str_len; iter++) {
			if (model_pointer->view_lines[i_j_main.i][i_j_main.j][iter] == ' ')
				break;
		}

		for (iter; iter < cur_str_len; iter++) {
			if (model_pointer->view_lines[i_j_main.i][i_j_main.j][iter] != ' ')
				break;
		}

		if (iter == cur_str_len) {
			if (beg_y >= MAX_SHOW_LINES_COUNT - 1) {
				this->change_first_line_pos(1);
				put_curs_on_wrap(beg_y, 0);
			}
			else {
				put_curs_on_wrap(++beg_y, 0);
			}
			return;
		}
		else {
			put_curs_on_wrap(beg_y, iter);
		}

	}

}


void View::go_to_line_N(char first_num) {
	MyString nummers = first_num;
	chtype press_res;
	while (1) {
		press_res = window_adapt->pressed_key();
		if (press_res != 'G' && press_res > 47 && press_res < 58)
			nummers += press_res;
		else
			break;
	}
	if (press_res != 'G')
		return;

	int str_num = 0, tens_pow = 1;
	for (int i = nummers.length() - 1; i > -1; i--) {
		str_num += tens_pow * (nummers[i] - 48);
		tens_pow *= 10;
	}

	if (str_num > model_pointer->view_lines.size() || str_num < 0)
		return;

	if (str_num >= model_pointer->view_lines.size() - MAX_SHOW_LINES_COUNT) {
		this->change_first_line_pos(model_pointer->view_lines.size() - MAX_SHOW_LINES_COUNT);
		str_num = str_num - (model_pointer->view_lines.size() - MAX_SHOW_LINES_COUNT + 1);
	}
	else {
		this->change_first_line_pos(str_num - this->first_line_position - 1);
		str_num = 0;
	}

	this->put_curs_on_wrap(str_num, 0);
}

void View::simple_cursor_set(int i, int j, int x) {
	int cur_y_in_file = 0;
	if (j != -1) {
		for (int k = 0; k < i; k++) {
			cur_y_in_file += model_pointer->view_lines[k].size();
		}
		cur_y_in_file += j;
	}
	else {
		cur_y_in_file = i;
	}

	if (first_line_position + MAX_SHOW_LINES_COUNT - 1 < cur_y_in_file || cur_y_in_file < first_line_position) { // экран нужно куда-то двигать
		if (cur_y_in_file < first_line_position && cur_y_in_file > -1) {
			change_first_line_pos(-(first_line_position - cur_y_in_file));
			put_curs_on_wrap(cur_y_in_file - first_line_position, x);
		}
		else if (first_line_position + MAX_SHOW_LINES_COUNT - 1 < cur_y_in_file) {
			if (cur_y_in_file > model_pointer->show_num_of_view_lines() - MAX_SHOW_LINES_COUNT - 1) {
				change_first_line_pos(model_pointer->show_num_of_view_lines() - MAX_SHOW_LINES_COUNT - first_line_position);
			}
			else
				change_first_line_pos(cur_y_in_file - (first_line_position + MAX_SHOW_LINES_COUNT - 1));
			put_curs_on_wrap(cur_y_in_file - first_line_position, x);
		}
	}
	else {
		put_curs_on_wrap(cur_y_in_file - first_line_position, x);
	}
}

void View::set_first_line_position_to_zero() {
	first_line_position = 0;
}

void View::show_help_window() {
	clear_whole_screen();
	this->window_adapt->str_add("Programm VIM == Vyacheslav Ilyich Mosin :)", 0, 0);

	this->window_adapt->str_add("Modes:", 2, 0);
	this->window_adapt->str_add("Navigation, Command, Find", 3, 0);
	
	this->window_adapt->str_add("Navigation:", 5, 0);
	this->window_adapt->str_add("RIGHT, LEFT, UP, DOWN, PAGE_UP, PAGE_DOWN, gg-to first str, G-to last str", 6, 0);
}
