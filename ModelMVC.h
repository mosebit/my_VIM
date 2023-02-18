#pragma once
//#pragma comment(lib, "MyString")
#include <vector>
#include <fstream>
#include "ViewMVC.h"
#include "MyString.h"
#include <utility>

using namespace std;
struct word_edges { int word_beg; int word_end; };

class View;
class Model {
public:
	Model(const char* path);
	~Model();

	void set_file_name();
	void read_from_file(MyString path);
	void write_to_file(const char* file_name);
	void close_file();
	void view_to_model(View* view_arg);

	MyString get_line(int y);
	int get_line_len(int y);
	int get_num_of_file_lines();
	MyString get_file_name();

	void delete_substr(int y, int x, int num_to_del);
	void connect_lines(int y_to_del, int y_to_expand);		// expand - расширить

	void prepare_lines_for_view();
	int show_num_of_view_lines();
	void update_num_of_view_lines();
	void split_lines();
	void delete_str_contents_by_num_in_view(int str_n);
	void update_view_lines(int module_str_n);
	void delete_symbol_by_view_x_y_pos(int cur_y, int cur_x);
	void change_symbol_by_view_x_y_pos(int view_y, int cur_x, char new_symbol);
	void insert_symbol_by_view_x_y_pos(int view_y, int cur_x, char new_symbol);

	void delete_char_after(int cur_y, int cur_x);
	void delete_word_after(int cur_y, int cur_x);

	word_edges get_word_edges(int cur_y, int cur_x);
	void copy_word(int cur_y, int cur_x);
	void copy_line(int cur_y);
	void copy_and_cut_line(int cur_y);
	int find_x_pos_in_file_lines(int cur_y, int cur_x);
	void insert_after_cursor(int cur_y);

	// searching
	void find_next(int y_pos, int x_pos, MyString searching_pair);
	void find_previous(int y_pos, int x_pos, MyString searching_pair);
	pair<int, int> search_j_x(int i, int file_x);
	void prev_find_coordinates(int i, int j, int x);
	void save_coordinates_put_cursor(pair<int, int> new_j_x, int i, int res);

	// new file, save file and so on
	void model_open_new_file(const char* path);

	// Public Data
	std::vector <std::vector <MyString>> view_lines;
	MyString searching_str;
	pair<pair<int, int>, int> i_j_prev;
private:
	MyString file_path;
	MyString file_name;
	MyString buffer;
	std::vector <MyString> file_lines;
	std::ifstream ifstr_model;
	View* view_p;
	int num_of_view_lines;	// кол-во строк, модифицированных по ширине экрана
};