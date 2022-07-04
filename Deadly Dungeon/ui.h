#pragma once
#include "data.h"

//Structs

struct UI
{
	//General


	//Main Menu


	//Gameplay
	//Sprites
	sf::Sprite heartSprite{};

	//Initialize UI
	void Init(const GameData& game);

	//Render all UI elements
	void Render(const GameData& game, sf::RenderWindow& window, const short& health, const short& coins);

	//Changes the health bar's texture
	void RenderHealthBar(const GameData& game, sf::RenderWindow& window, const short& health);
};

//Game constants
namespace GC
{
	enum HEARTS { HEART_EMPTY, HEART_HALF, HEART_FULL }; //Health bar heart states

	//UI: General
	const char UI_BORDER = 8;
	//UI: Coins
	const sf::IntRect COINS_ANIM_RECT = { 288, 272, 32, 8 };
	const unsigned char COINS_FRAMES = 4;
	const Dim2Di COINS_ORIGIN = { 4, 5 };
	//UI: Health
	const Dim2Di HEART_DIMENSIONS = { 16, 13 };
	const sf::IntRect HEART_FULL_TEXTURE_RECT = { 288, 257, HEART_DIMENSIONS.x, HEART_DIMENSIONS.y };
	const sf::IntRect HEART_HALF_TEXTURE_RECT = { 304, 257, HEART_DIMENSIONS.x, HEART_DIMENSIONS.y };
	const sf::IntRect HEART_EMPTY_TEXTURE_RECT = { 320, 257, HEART_DIMENSIONS.x, HEART_DIMENSIONS.y };
	const Dim2Di HEART_ORIGIN = { 8, 7 };
}