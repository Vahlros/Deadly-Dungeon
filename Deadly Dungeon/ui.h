#pragma once
#include "data.h"
#include "graphics.h"

struct Button
{
	int ID = 0;
	bool mouseHover = false;
	sf::Sprite sprite{};
};

struct GameUI
{
	//Health
	sf::Sprite heartSprite{};

	//Coins
	sf::Sprite coinSprite{};
	sf::Text coinText{};
	Animation coinAnim{};

	//Time
	sf::Text timeText{};

	//Initialize UI
	void Init(const GameData& game);

	//Render all UI elements
	void Render(const GameData& game, sf::RenderWindow& window, const short& health, const short& coins);

	//Renders the health bar by altering the texture and position of heartSprite
	void RenderHealthBar(const GameData& game, sf::RenderWindow& window, const short& health);

	//Renders an animated coin and the player's current amount
	void RenderCoins(const GameData& game, sf::RenderWindow& window, const short& coins);

	//Renders formatted time in the top right of the view
	void RenderTime(const GameData& game, sf::RenderWindow& window);
};

//Game constants
namespace GC
{
	//Enums
	enum HEARTS { HEART_EMPTY, HEART_HALF, HEART_FULL }; //Health bar heart states
	enum BUTTONS { B_BACK, B_PLAY, B_SCORES, B_TUTORIAL };

	//UI: General
	const int UI_BORDER = 8;
	const int UI_TIME_BORDER = 32;
	//UI: Health
	const Dim2Di HEART_DIMENSIONS = { 16, 13 };
	const sf::IntRect HEART_FULL_TEXTURE_RECT = { 288, 257, HEART_DIMENSIONS.x, HEART_DIMENSIONS.y };
	const sf::IntRect HEART_HALF_TEXTURE_RECT = { 304, 257, HEART_DIMENSIONS.x, HEART_DIMENSIONS.y };
	const sf::IntRect HEART_EMPTY_TEXTURE_RECT = { 320, 257, HEART_DIMENSIONS.x, HEART_DIMENSIONS.y };
	const Dim2Df HEART_ORIGIN = { 8.f, 7.f };
}