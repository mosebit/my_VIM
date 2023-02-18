#pragma once	// не допускает возможности коллизии имён
#include "ModelMVC.h"
//#include "ControllerMVC.h"
#include "MyString.h"
#include "AdapterCurses.h"

#include <vector>
using namespace std;

#define MAX_SHOW_LINES_COUNT		26
#define MOVE_CURSOR_ON_LAST_STR_POS -1 // при получении такого значения курсор перенесется на последнюю позицию строки

static struct i_j_struct {
	int i;
	int j;
};

class Model;
class View
{
public:
	View(Model* model_p);
	~View();

	void txt_to_window();
	void update_page_content(int start_line);
	void change_first_line_pos(int size_of_changes);
	int get_y_pos();
	int get_x_pos();
	void statbar_update(MyString* nav_str, int y);
	chtype get_pressed_key();
	void moving_cursor_or_page(chtype mode);
	void edit_cursor_x_pos(int new_x_pos);		// updated
	AdapterCurses* show_adapter();
	int show_first_line_position() { return this->first_line_position; }
	void put_curs_on_wrap(int y, int x) { this->window_adapt->put_cursor_on(y, x); }

	void new_txt_to_window();
	void new_update_page_content(int start_line);
	struct i_j_struct find_i_j_in_view_strings(int view_str_num);
	void search_pos_to_move(char left_right_b_w);
	void go_to_line_N(char first_num);
	void simple_cursor_set(int i, int j, int x); // двигает экран, если нужно и устанавливает курсор по координатам
	void set_first_line_position_to_zero();
	void clear_whole_screen();
	void show_help_window();
private:
	int first_line_position;
	Model* model_pointer;
	AdapterCurses* window_adapt;
};