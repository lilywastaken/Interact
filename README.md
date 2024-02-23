# Interract

# An interraction program to interract with windows

This tiny yet very useful tool allows you to do anything with any window on your computer:
- Acquire RGB value of a pixel
- Click using mouse
- Drag&drop
- etc!

It is currently set to detect the window "Chess", which you can get here:
https://github.com/lilywastaken/Chess

Just open the window, make sure the window is not minimized (in the background). Doesn't matter if other windows are on top of it as long as the name you're looking for is not also included in them.
This program should check if the bottom-left knight is in. If he is, a click pushes him/her/them (idk) outside. If not, he's brought inside.

Wanna do your own things ? Just change "WINDOW_NAME" to your target and adjust the commands to run.


To compile the code, make sure you've installed the following:

sudo apt install g++
sudo apt install wmctrl
sudo apt install xdotool
sudo apt install libopencv-dev


And run the command:

g++ interract.cpp -o interract `pkg-config --cflags opencv4` `pkg-config --libs opencv4`

