#include "menu.h"
#include "input.h"

//This is to force laptops to use the dedicated gpu instead of defaulting to the integrated gpu
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;

//To find out how many lines of code, Ctrl + Shift + F, type "\n" in the search box and enable regular expressions

// !!!! USEFUL SHIT !!!! LEARN THESE !!!!
//Click on symbol name and press F12 to go to definition
//Ctrl + Left click = the same as F12
//Crtl + K and Crtl + O to swap between header and source file
//Crtl-Shift-B to build the project
//F5 to build and run the project
//To change multiple symbols at once:
//SHIFT + ALT + . = Select occurence
//SHIFT + ALT + , = Deselect 1 occurence
//Shift + Alt + Up Arrow = Select vertically upwards
//Shift + Alt + Down Arrow = Select vertically downwards
//CTRL + R, CTRL + R = Rename variable
//CTRL + , = Search for thing in project
//CTRL + K, CTRL + C = Comment
//CTRL + K, CTRL + U = uncomment
//CRTL + SHIFT + V - Clipboard history
// SHIFT + END selects up to the end of the line
// SHIFT + HOME selects up to the beginning of the line
// !!!! FUCKIN LEARN THESE !!!!

void GetCursorImage(sf::Cursor& cursor, sf::RenderWindow& window)
{
	sf::Image tempCursor;
	tempCursor.create(GC::CURSOR_DIMENSIONS.x, GC::CURSOR_DIMENSIONS.y);
	tempCursor.loadFromFile("graphics/cursor.png");

	if (cursor.loadFromPixels(tempCursor.getPixelsPtr(), sf::Vector2u(GC::CURSOR_DIMENSIONS), { 0, 0 }))
	{
		window.setMouseCursor(cursor);
	}
}

int main()
{
	//Window
	sf::RenderWindow window;
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Deadly Dungeon", sf::Style::Fullscreen);
	window.setFramerateLimit(GC::FRAMERATE);

	//Cursor
	sf::Cursor cursor;
	GetCursorImage(cursor, window);

	//State Manager
	StateManager state;
	state.Init(window);
	state.MainLoop(window);
}