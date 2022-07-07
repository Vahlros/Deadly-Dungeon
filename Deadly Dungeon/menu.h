#pragma once
#include "game.h"

struct Menu
{
	//Main stats
	int ID = -1;
	std::vector <Button> buttons;

	//SFML
	sf::Texture background;
	sf::Sprite sprite;

	//
	//void CheckButtonHover();
};

namespace GC //Game constants
{
	//Enums
	enum MENUS { MENU_MAIN, MENU_SCORES, MENU_TUTORIAL };
	
	//Menus: General
	const int NUM_MENUS = 3;
	const int NUM_MENU_TEXTURES = 4;
}

void InitializeMenus(std::vector <Menu>& menus);

void MenusInputHandling(sf::RenderWindow& window, Player& player, const int& state, GameData& game, std::vector<Room>& rooms);

void GameInputHandling();