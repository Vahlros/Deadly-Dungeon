#pragma once
#include "game.h"

struct Button
{
	int ID = 0;
	bool mouseHover = false;
	sf::Sprite sprite{};
};

struct Menu
{
	//Main stats
	int ID = -1; //Menu ID
	int fromID = -1; //Menu ID of the previous menu
	std::vector <Button> buttons;

	//SFML
	sf::Sprite sprite;

	//Changes colour and sets mouseHover bool to true if the mouse is hovering over the button
	void UpdateButtonHover(const Dim2Df& mousePosition, sf::Sound& hoverSound);

	void CheckClick(const Input& input, int& state, bool& startGame, sf::Sound& selectSound);

	void Render(sf::RenderWindow& window);
};

namespace GC //Game constants
{
	//Enums
	enum MENUS { MENU_MAIN, MENU_SCORES, MENU_CONTROLS };
	
	//Menus: General
	const int NUM_MENUS = 3;
	const int NUM_MENU_TEXTURES = 7;
	//Menus: Main Menu
	const int MAIN_MENU_NUM_BUTTONS = 3;
	const Dim2Df MAIN_PLAY_POSITION = { 1460.f, 611.f };
	const Dim2Df MAIN_SCORES_POSITION = { 1355.f, 769.f };
	const Dim2Df MAIN_CONTROLS_POSITION = { 1222.f, 930.f };
	const Dim2Df BACK_POSITION = { 1587.f, 36.f };
}

void InitializeMenus(std::vector <sf::Texture>& menuTextures, std::vector <Menu>& menus);

void MenusInputHandling(sf::RenderWindow& window, int& state, const Input& input, std::vector<Menu>& menus, Game& game, bool& startGame);

void RenderMenus(sf::RenderWindow& window, std::vector<Menu>& menus, int& state);