#pragma comment(lib, "MyString")
#include "ViewMVC.h"
#include "ControllerMVC.h"

int main(int argc, char* argv[]) {
	Model model_0(argv[1]);
	View view_0(&model_0);
	model_0.prepare_lines_for_view();
	view_0.txt_to_window();
	Controller controller_0(&model_0, &view_0);
	return 0;
}