#include "AdapterCurses.h"

AdapterCurses::AdapterCurses() {
	initscr();						// initialize all curses data structures, and arrange that the first call to refresh() will clear the screen
	my_window = newwin(0, 0, 0, 0);	// Create a new full-screen window by this calling (int nlines, int ncols, int begy, int begx)
	noecho();						// control whether typed characters are echoed by the input routine
	scrollok(my_window, TRUE);
	keypad(my_window, TRUE);		// if is TRUE, and a function key is pressed, the token for that function key will be returned instead of the raw characters
	wrefresh(my_window);			// copies the named window to the physical terminal screen, taking into account what is already there in order to optimize cursor movement.
}
AdapterCurses::~AdapterCurses() {
	delwin(my_window);				// deletes the named window, freeing all associated memory
}

chtype AdapterCurses::pressed_key() {
	return wgetch(my_window);
}
void AdapterCurses::str_add(const char* str, int y, int x) {
	mvwaddstr(my_window, y, x, str);
	wrefresh(my_window);
}
int AdapterCurses::get_cur_y_pos() {
	wrefresh(my_window);
	int y = 0;
	int x = 0;
	getsyx(y, x);
	return y;
}
int AdapterCurses::get_cur_x_pos() {
	int y = 0;
	int x = 0;
	getsyx(y, x);
	return x;
}
void AdapterCurses::put_cursor_on(int y, int x) {
	this->my_window->_curx = x;
	this->my_window->_cury = y;
	wrefresh(my_window); // под вопросом нужно ли это здесь, вроде нужно
}
int AdapterCurses::max_x() {
	return this->my_window->_maxx;
}
int AdapterCurses::max_y() {
	return this->my_window->_maxy;
}

void AdapterCurses::del_line(int y) {
	//mvwdelch(this->my_window, y, 0);
	//werase(this->my_window);
	//winsdelln(this->my_window, y); // удаляется не так строка
	for (int _x = 0; _x < max_x(); _x++) {
		mvwinsch(this->my_window, y, _x, ' ');
	}
}
