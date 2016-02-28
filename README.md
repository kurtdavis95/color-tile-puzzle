Kurt Davis
2/25/16

OpenGL implementation of 4x1 tile color matching puzzle. 
Puzzle equivalent to regular language problem.

To build:
	-Install required libraries (listed below)
	-$make all

	-$make clean removes executable.

Upon running the executable, the program displays
the puzzle described in boardtile.h and boardedge.h This includes the puzzle’s frame, the 4 available
tiles, and a grey box highlighting the selected tile. This grey box is the user’s means of
interacting with the program. When hovering over a tile in the bottom row, pressing number
keys 1, 2, 3, and 4 will assign the selected tile to the respective position in the frame (1 being far
left and 4 being far right). The grey box (referred to as the selector) is moved with the
keyboard’s arrow keys, and can be positioned over any tiles in the bottom row or tile spaces in
the frame. When selecting a tile placed in the frame, pressing ‘e’ will rotate the tile ninety
degrees clockwise, and pressing ‘q’ will rotate the tile ninety degrees counterclockwise.
Pressing ‘w’ will remove the tile from the space. The tiles shown at the bottom cannot be
rotated or modified, but more than one of each can be assigned to a space in the frame. To quit
the application, press the Escape key. The program can be configured to represent any 1x4
puzzle like the one shown, using the colors red, blue, green, and yellow. The color sets are
encoded in the define statements at the top of main.cpp, as strings of characters ‘r’, ‘g’, ‘b’, and
‘y’.

Libraries Used:
OpenGL:	(should be included with video drivers)
SDL2:	https://www.libsdl.org/download-2.0.php
	OR
	apt-get install libsdl2-dev
	(using instructions at: http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php)

GLEW:	http://glew.sourceforge.net/index.html
	(using instructions at http://glew.sourceforge.net/build.html
	and http://glew.sourceforge.net/install.html)
