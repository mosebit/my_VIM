
my_VIM - моя учебная задача, целью является написание аналога текстового редактора VIM на основе самописного класса MyString - аналога класса String в языке C++.
В моей программе подготовки ООП преподается на английском языке, так что далее все будет на английском.

As a result of working on this project I get the application with UML class diagram:
![Снимок экрана 2023-02-18 164855](https://user-images.githubusercontent.com/56561519/219869767-6ed881ac-77c8-480a-856d-1ee33570f431.jpg)

**User’s guide:**<br/>
The app supports 4 modes of operation:
-	navigation and editing — move the cursor over the text, scroll through the pages, go to the desired line, edit the text: copy, paste, cut a line/word. When you enter command buttons, they are NOT displayed on the screen;
-	text input — regular text input. Backspace, delete, insert, and all navigation actions should work;
-	command input — the command input is displayed on the command line. Backspace should work on the command line;
-	search — as command mode, but for search.
Navigation mode — the main mode of operation, from this mode you can switch to "input Mode", "command Mode" and "search Mode". When you start the program, it starts working in this mode.

**You can use next buttons to change mode:**
i -	Enter text before the cursor.<br/>
I	- Go to the beginning of the line and start entering text.<br/>
S	- Go to the end of the line and start typing.<br/>
A	- Delete the contents of the row and start typing.<br/>
r	- Replace one character under the cursor.<br/>
:	- Activating the command input mode.<br/>
/	- Activates search mode until the end of the document.<br/>
?	- Activates search mode until the start of the document.<br/>

**In Navigation mode use buttons:**<br/>
RIGHT   - Moves the cursor one position to the right if it is the end of the line moves to the first position of the next line.<br/>
LEFT    - Moves the cursor one position to the left if this is the beginning of the line moves to the end of the previous line.<br/>
UP      - Moves the cursor up one position (if next line shorter, then moves to the end of this line) if this is the beginning of the screen doing a scroll.<br/>
DOWN    - Moves the cursor one position (if next line shorter, then moves to the end of this line) down if it is the end of the screen doing a scroll.<br/>
PAGE_UP   -	Raises the cursor one page.<br/>
PAGE_DOWN -	Moves the cursor down one page.<br/>
gg        -	Moves the cursor to the first page of the text.<br/>
G	        -Moves the cursor to the last page of the text.<br/>

To enter in «Find mode» use Command «/<text>» - to search forward and «?<text>» - to search back. To repeat search you can use buttons «n» - to repeat search forward and «N» - to repeat search back.<br/><br/>
**In «Command mode» available commands:<br/>**
o <filename>	Open the file <filename>.<br/>
x	Write to the current file and close it.<br/>
w	Write to the current file.<br/>
w <filename>	Write to file <filename>.<br/>
q	Exit program.<br/>
q!	Exit program without saving current file.<br/>
wq!	Write to the current file and close it.<br/>
num	Go to the line with the number <num>.<br/>
h	Program help.<br/>

