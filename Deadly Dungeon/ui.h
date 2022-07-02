#pragma once
#include "data.h"

//Structs

struct UI
{
	//Main Menu
	//All main menu's variables and functions go here

	//Playing Game
	//Textures
	sf::Texture healthBar{};
	sf::Texture itemBar{};


	//Sprites
	sf::Sprite healthBarSprite{};
	sf::Sprite itemBarSprite{};

	//Text
	sf::Font font{};

	//Initialize UI
	void Init(const sf::Image& spritesheet);

	//Render all UI elements
	void Render(sf::RenderWindow& window);

	//Changes the health bar's texture
	void UpdateHealthBar();

	//Changes the item bar's texture
	void UpdateItemBar();
};

//Game constants
namespace GC
{
	//Font
	const int TEXT_SIZE = 36;
}