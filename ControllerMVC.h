#pragma once
#include <curses.h>
#include <iostream>
#include "ViewMVC.h"
#include "AdapterCurses.h"
#include <utility>

// Key Codes
#define K_DOWN	258
#define K_UP		259
#define K_RIGHT	261
#define K_LEFT	260
#define K_PAGE_DOWN	338
#define K_PAGE_UP		339
// 
#define K_BACKSPC		0x08
#define K_DELETE		0x14A
#define K_ESC			0x1B
#define K_ENTER			0x0A


class Controller
{
public:
	Controller(Model* model_pointer, View* view_pointer);
	~Controller();
	void set_adapter();

	void navigation_mode();
	void write_mode(chtype mode);
	void text_editing();
	void find_mode(chtype mode);
	void command_mode();
	void regular_stat_bar(MyString* str);
	void status_bar_upd(short mode);
	//void help_window();
	void navig_press_waiting();
	void move_cursor_page(chtype mode);
	void edit_one_symbol();
	pair<MyString, char> scan_str();

	void open_new_file(MyString file_name);
private:
	Model* my_model;
	View* my_View;
	AdapterCurses* my_adapt;
};

