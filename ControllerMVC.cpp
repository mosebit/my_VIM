#include "ControllerMVC.h"
#include <iostream>
#include <sstream>
using namespace std;

#define NAVIGATION_MODE 1
#define WRITE_MODE		2
#define COMMAND_MODE	3
#define FIND_MODE		4

Controller::Controller(Model* model_pointer, View* view_pointer) {
	my_model = model_pointer;
	my_View = view_pointer;
	set_adapter();
	navigation_mode();
}
Controller::~Controller() {

}
void Controller::set_adapter() {
	this->my_adapt = my_View->show_adapter();
}

void Controller::navigation_mode() {
	while (1) {
		status_bar_upd(NAVIGATION_MODE);
		navig_press_waiting();
	}
}
void Controller::navig_press_waiting() {
	chtype press_res = this->my_adapt->pressed_key();
	switch (press_res)
	{
		// ------------------------ >>> TABLE 1 - Change mode buttons
	case 'i':
	case 'I':
	case 'S':
	case 'A':
	case 'r':
		write_mode(press_res);
		break;
	case ':':
		command_mode();
		break;
	case '/':
	case '?':
	case 'n':
	case 'N':
		find_mode(press_res);
		break;
		// ------------------------ <<< TABLE 1

		// ------------------------ >>> TABLE 2 - Navigation buttons
	case K_DOWN:
	case K_UP:
	case K_RIGHT:
	case K_LEFT:
	case K_PAGE_DOWN:
	case K_PAGE_UP:
	case 'g':
	case 'G':
	case '^':
	case '$':
	case 'w':
	case 'b':
		my_View->moving_cursor_or_page(press_res);
		break;
		// ------------------------ <<< TABLE 2

		// ------------------------ >>> TABLE 3 - Navigation command buttons
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		this->my_View->go_to_line_N(press_res);
		break;
		// edit
	case 'x':
		my_model->delete_char_after(my_View->show_first_line_position() + my_View->get_y_pos(), my_View->get_x_pos());
		break;
	case 'd': {
		chtype new_press_res = this->my_adapt->pressed_key();
		if (new_press_res == 'i') {
			new_press_res = this->my_adapt->pressed_key();
			if (new_press_res == 'w') {
				my_model->delete_word_after(my_View->show_first_line_position() + my_View->get_y_pos(), my_View->get_x_pos());
			}
			else
				break;
		}
		else if (new_press_res == 'd') {
			my_model->copy_and_cut_line(my_View->show_first_line_position() + my_View->get_y_pos());
		}

		break;
	}
	case 'y': {
		chtype new_press_res = this->my_adapt->pressed_key();
		if (new_press_res == 'w') {
			my_model->copy_word(my_View->show_first_line_position() + my_View->get_y_pos(), my_View->get_x_pos());
		}
		else {
			my_model->copy_line(my_View->show_first_line_position() + my_View->get_y_pos());
		}
		break;
	}
	case 'p':
		my_model->insert_after_cursor(my_View->show_first_line_position() + my_View->get_y_pos());
		break;
		// ------------------------ <<< TABLE 3

	default:
		break;
	}
}


void Controller::write_mode(chtype mode) {
	switch (mode)
	{
	case 'i':
		this->text_editing();
		break;
	case 'I':
		this->my_adapt->put_cursor_on(this->my_View->get_y_pos(), 0);
		this->text_editing();
		break;
	case 'S':
		this->my_adapt->put_cursor_on(this->my_View->get_y_pos(),
			this->my_model->get_line_len(this->my_View->show_first_line_position() + this->my_View->get_y_pos()));
		this->text_editing();
		break;
	case 'A': {
		this->my_model->delete_str_contents_by_num_in_view(this->my_View->show_first_line_position() + this->my_View->get_y_pos());
		int x_beg_pos = this->my_View->get_x_pos(), y_beg_pos = this->my_View->get_y_pos();
		this->my_View->new_update_page_content(this->my_View->show_first_line_position());
		this->my_View->put_curs_on_wrap(y_beg_pos, x_beg_pos);
		this->text_editing();
		break;
	}
	case 'r':
		this->edit_one_symbol();
		this->navigation_mode();
		break;

	default:
		break;
	}
}

void Controller::edit_one_symbol() {
	status_bar_upd(WRITE_MODE);

	chtype press_res = this->my_adapt->pressed_key();
	int x_position = this->my_View->get_x_pos();
	int y_position = this->my_View->get_y_pos();

	switch (press_res)
	{
		// ------------------------ >>> TABLE 2 - Navigation buttons
	case K_DOWN:
	case K_UP:
	case K_RIGHT:
	case K_LEFT:
	case K_PAGE_DOWN:
	case K_PAGE_UP:
	case K_BACKSPC:
	case K_ESC:
	case K_DELETE:
		break;
		//move_cursor_page(press_res);press_res
	// ------------------------ <<< TABLE 2

	case K_ENTER:
		this->my_model->delete_symbol_by_view_x_y_pos(this->my_adapt->get_cur_y_pos() + this->my_View->show_first_line_position(),
			this->my_adapt->get_cur_x_pos());
		this->my_model->split_lines();
		this->navigation_mode();
		break;

	default: {
		this->my_model->change_symbol_by_view_x_y_pos(this->my_adapt->get_cur_y_pos() + this->my_View->show_first_line_position(),
			this->my_adapt->get_cur_x_pos(), press_res);
		int x_beg_pos = this->my_View->get_x_pos(), y_beg_pos = this->my_View->get_y_pos();
		this->my_View->new_update_page_content(this->my_View->show_first_line_position());
		this->my_View->put_curs_on_wrap(y_beg_pos, x_beg_pos);
		break;
	}
	}
}

void Controller::text_editing() {
	while (1) {
		status_bar_upd(WRITE_MODE);

		chtype press_res = this->my_adapt->pressed_key();
		int x_position = this->my_View->get_x_pos();
		int y_position = this->my_View->get_y_pos();

		switch (press_res)
		{
			// ------------------------ >>> TABLE 2 - Navigation buttons
		case K_DOWN:
		case K_UP:
		case K_RIGHT:
		case K_LEFT:
		case K_PAGE_DOWN:
		case K_PAGE_UP:
			my_View->moving_cursor_or_page(press_res);
			break;
			//move_cursor_page(press_res);press_res
		// ------------------------ <<< TABLE 2

		case K_ENTER:
			this->my_model->split_lines();
			break;

		case K_BACKSPC:

			break;

		case K_DELETE:

			break;

		case K_ESC:
			this->navigation_mode();
			break;

		default: {
			this->my_model->insert_symbol_by_view_x_y_pos(this->my_adapt->get_cur_y_pos() + this->my_View->show_first_line_position(),
				this->my_adapt->get_cur_x_pos(), press_res);
			int x_beg_pos = this->my_View->get_x_pos(), y_beg_pos = this->my_View->get_y_pos();
			this->my_View->new_update_page_content(this->my_View->show_first_line_position());
			i_j_struct i_j_view;
			i_j_view = this->my_View->find_i_j_in_view_strings(y_beg_pos);
			if (x_beg_pos >= COLS - 1 && i_j_view.j < this->my_model->view_lines[i_j_view.i].size()) {
				x_beg_pos = 1;
				if (y_beg_pos == MAX_SHOW_LINES_COUNT)
					y_beg_pos++;
				else
					this->my_View->change_first_line_pos(1);
			}
			else {
				x_beg_pos++;
			}

			this->my_View->put_curs_on_wrap(y_beg_pos, x_beg_pos);
			break;
		}
		}
	}
}
pair<MyString, char> Controller::scan_str() {
	pair<MyString, char> res;
	chtype new_press_res;
	MyString str_num;
	while (1) {
		new_press_res = this->my_adapt->pressed_key();
		if (new_press_res != K_ENTER && new_press_res > 31 && new_press_res < 127) {
			str_num += new_press_res;
		}
		else
			break;
	}
	res.first = str_num;
	res.second = new_press_res;	// последняя нажатая клавиша
	return res;
}

void Controller::open_new_file(MyString file_name) {
	my_model->model_open_new_file(file_name.c_str());
	my_View->set_first_line_position_to_zero();
	my_View->put_curs_on_wrap(0, 0);
	my_model->prepare_lines_for_view();
	my_View->txt_to_window();
	my_View->clear_whole_screen();
	my_View->new_update_page_content(0);
	navigation_mode();
}

void Controller::command_mode() {
	while (1) {
		status_bar_upd(COMMAND_MODE);
		chtype press_res = this->my_adapt->pressed_key();
		switch (press_res)
		{
		case K_ESC:
			navigation_mode();
			break;

		case 'o': {
			pair <MyString, char> scan_res;
			scan_res = scan_str();
			if (scan_res.first.length() < 1)
				break;
			open_new_file(scan_res.first.c_str());
			break;
		}
		case 'x': {
			my_model->write_to_file(NULL);
			my_model->close_file();
			break;
		}
		case 'w': {
			pair <MyString, char> scan_res;
			scan_res = scan_str();
			if (scan_res.first.length() < 1) {
				my_model->write_to_file(NULL);
			}
			else if (scan_res.first.length() == 2 && scan_res.first[0] == 'q' && scan_res.first[1] == '!') {
				my_model->write_to_file(NULL);
				my_model->close_file();
			}
			else {
				my_model->write_to_file(scan_res.first.c_str());
			}
			//open_new_file(scan_res.first.c_str());
			break;
		}
		case 'q': {
			pair <MyString, char> scan_res;
			scan_res = scan_str();
			if (scan_res.first.length() < 1) {
				my_model->write_to_file(NULL);
				my_model->close_file();
			}
			else if (scan_res.first.length() == 1 && scan_res.first == '!')
				my_model->close_file();
			break;
		}
		case 'h': {
			my_View->show_help_window();
			chtype new_press_res = this->my_adapt->pressed_key();
			my_View->clear_whole_screen();
			my_View->update_page_content(my_View->show_first_line_position());
			break;
		}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			chtype new_press_res;
			MyString str_num = press_res;
			pair <MyString, char> scan_res;
			scan_res = scan_str();
			if (scan_res.second == K_ENTER)
				str_num += scan_res.first;
			else
				return;
			int str_num_int = 0, dec = 1;
			for (int i = str_num.length() - 1; i > -1; i--) {
				str_num_int += (str_num[i] - 48) * dec;
				dec *= 10;
			}
			str_num_int--;
			if (str_num_int < my_model->show_num_of_view_lines() && str_num_int > -1)
				my_View->simple_cursor_set(str_num_int, -1, 0);
			break;
		}

		default:
			break;
		}
	}
}

void Controller::find_mode(chtype mode) {
	status_bar_upd(FIND_MODE);
	if (mode == 'n') {
		if (my_model->i_j_prev.first.first != -1) {
			my_model->find_next(my_View->get_y_pos() + my_View->show_first_line_position(),
				++my_model->i_j_prev.second, my_model->searching_str);
		}
		return;
	}
	else if (mode == 'N') {
		if (my_model->i_j_prev.first.first != -1) {
			my_model->find_previous(my_View->get_y_pos() + my_View->show_first_line_position(),
				++my_model->i_j_prev.second, my_model->searching_str);
		}
		return;
	}

	chtype new_press_res;
	my_model->searching_str.clear();
	while (1) {
		new_press_res = this->my_adapt->pressed_key();
		if (new_press_res != K_ENTER && new_press_res > 31 && new_press_res < 127) {
			my_model->searching_str += new_press_res;
		}
		else
			break;
	}
	if (new_press_res != K_ENTER || my_model->searching_str.length() < 1)
		return;

	if (mode == '/')
		my_model->find_next(my_View->get_y_pos() + my_View->show_first_line_position(), my_View->get_x_pos(), my_model->searching_str);
	else if (mode == '?')
		my_model->find_previous(my_View->get_y_pos() + my_View->show_first_line_position(), my_View->get_x_pos(), my_model->searching_str);

	// в зависимости от "chtype mode" начинать поиск либо в конец, либо к началу файла
}


void Controller::regular_stat_bar(MyString* navbar_str) { // подготавливает строку, которая будет одинаковой для всех режимов
	string tmp_str;
	*navbar_str = "File Name: ";
	*navbar_str += my_model->get_file_name();
	*navbar_str += "\tNum of all nums: ";
	tmp_str = to_string(my_model->get_num_of_file_lines());
	*navbar_str += tmp_str;
	*navbar_str += "\tCurrent line: ";
	tmp_str = to_string(my_View->get_y_pos() + this->my_View->show_first_line_position() + 1);
	*navbar_str += tmp_str;
}

void Controller::status_bar_upd(short mode) {
	MyString navbar_str;
	int line_for_print = 0;
	string tmp_str;

	switch (mode)
	{
	case NAVIGATION_MODE:
		this->regular_stat_bar(&navbar_str);
		navbar_str += "\tNAVIGATION_MODE";
		//line_for_print = my_model->get_num_of_file_lines() + 1;
		line_for_print = MAX_SHOW_LINES_COUNT + 2;
		my_View->statbar_update(&navbar_str, line_for_print);
		break;

	case WRITE_MODE:
		this->regular_stat_bar(&navbar_str);
		navbar_str += "\tWRITE_MODE";
		line_for_print = MAX_SHOW_LINES_COUNT + 2;
		my_View->statbar_update(&navbar_str, line_for_print);
		break;

	case COMMAND_MODE:
		this->regular_stat_bar(&navbar_str);
		navbar_str += "\tCOMMAND_MODE";
		line_for_print = MAX_SHOW_LINES_COUNT + 2;
		my_View->statbar_update(&navbar_str, line_for_print);
		break;

	case FIND_MODE:
		this->regular_stat_bar(&navbar_str);
		navbar_str += "\tFIND_MODE";
		line_for_print = MAX_SHOW_LINES_COUNT + 2;
		my_View->statbar_update(&navbar_str, line_for_print);
		break;

	default:
		break;
	}
}
void help_window() {

}