#pragma once
#include <curses.h>

class AdapterCurses
{
public:
	AdapterCurses();
	~AdapterCurses();
	
	void str_add(const char* str, int y, int x);
	void del_line(int y);
	chtype pressed_key();

	int get_cur_y_pos();
	int get_cur_x_pos();

	void put_cursor_on(int y, int x);

	int max_x();
	int max_y();
private:
	WINDOW* my_window;	// WINDOW - two-dimensional array of characters representing all or part of a terminal’s screen
	//int curs_x;
	//int curs_y;
};