#include "ui.h"

//Edits a char[] to represent the current state of the player's health
void GetHealthBarState(char hearts[], const short& health)
{
	//Get a list of health states, split into 4 to reduce repetition
	if (health >= (GC::BOOSTED_HEALTH * 3 / 4)) //At least 12
	{
		hearts[0] = GC::HEART_FULL;
		hearts[1] = GC::HEART_FULL;
		hearts[2] = GC::HEART_FULL;
		hearts[3] = GC::HEART_FULL;
		hearts[4] = GC::HEART_FULL;
		hearts[5] = GC::HEART_FULL;

		switch (health)
		{
		case 16:
			hearts[6] = GC::HEART_FULL;
			hearts[7] = GC::HEART_FULL;
			break;

		case 15:
			hearts[6] = GC::HEART_FULL;
			hearts[7] = GC::HEART_HALF;
			break;

		case 14:
			hearts[6] = GC::HEART_FULL;
			hearts[7] = GC::HEART_EMPTY;
			break;

		case 13:
			hearts[6] = GC::HEART_HALF;
			hearts[7] = GC::HEART_EMPTY;
			break;

		case 12:
			hearts[6] = GC::HEART_EMPTY;
			hearts[7] = GC::HEART_EMPTY;
			break;
		}
	}
	else if(health >= (GC::BOOSTED_HEALTH / 2)) //At least 8, less than 12
	{
		hearts[0] = GC::HEART_FULL;
		hearts[1] = GC::HEART_FULL;
		hearts[2] = GC::HEART_FULL;
		hearts[3] = GC::HEART_FULL;

		switch (health)
		{
		case 11:
			hearts[4] = GC::HEART_FULL;
			hearts[5] = GC::HEART_HALF;
			break;

		case 10:
			hearts[4] = GC::HEART_FULL;
			hearts[5] = GC::HEART_EMPTY;
			break;

		case 9:
			hearts[4] = GC::HEART_HALF;
			hearts[5] = GC::HEART_EMPTY;
			break;

		case 8:
			hearts[4] = GC::HEART_EMPTY;
			hearts[5] = GC::HEART_EMPTY;
			break;
		}

		hearts[6] = GC::HEART_EMPTY;
		hearts[7] = GC::HEART_EMPTY;
	}
	else if (health >= (GC::BOOSTED_HEALTH / 4)) //At least 4, less than 8
	{
		hearts[0] = GC::HEART_FULL;
		hearts[1] = GC::HEART_FULL;

		switch (health)
		{
		case 7:
			hearts[2] = GC::HEART_FULL;
			hearts[3] = GC::HEART_HALF;
			break;

		case 6:
			hearts[2] = GC::HEART_FULL;
			hearts[3] = GC::HEART_EMPTY;
			break;

		case 5:
			hearts[2] = GC::HEART_HALF;
			hearts[3] = GC::HEART_EMPTY;
			break;

		case 4:
			hearts[2] = GC::HEART_EMPTY;
			hearts[3] = GC::HEART_EMPTY;
			break;
		}

		hearts[4] = GC::HEART_EMPTY;
		hearts[5] = GC::HEART_EMPTY;
		hearts[6] = GC::HEART_EMPTY;
		hearts[7] = GC::HEART_EMPTY;
	}
	else //Less than 4
	{
		switch (health)
		{
		case 3:
			hearts[0] = GC::HEART_FULL;
			hearts[1] = GC::HEART_HALF;
			break;

		case 2:
			hearts[0] = GC::HEART_FULL;
			hearts[1] = GC::HEART_EMPTY;
			break;

		case 1:
			hearts[0] = GC::HEART_HALF;
			hearts[1] = GC::HEART_EMPTY;
			break;

		case 0:
			hearts[0] = GC::HEART_EMPTY;
			hearts[1] = GC::HEART_EMPTY;
			break;
		}

		hearts[2] = GC::HEART_EMPTY;
		hearts[3] = GC::HEART_EMPTY;
		hearts[4] = GC::HEART_EMPTY;
		hearts[5] = GC::HEART_EMPTY;
		hearts[6] = GC::HEART_EMPTY;
		hearts[7] = GC::HEART_EMPTY;
	}
}

//Renders heart sprite on a loop, according to a 
void RenderHearts(sf::RenderWindow& window, sf::Sprite& heartSprite, char hearts[], const char& numHearts)
{
	Dim2Df moveVector = { (float)GC::HEART_DIMENSIONS.x, 0.f };

	for (char index = 0; index < numHearts; index++)
	{
		switch (hearts[index])
		{
		case GC::HEART_EMPTY:
			heartSprite.setTextureRect(GC::HEART_EMPTY_TEXTURE_RECT);
			break;

		case GC::HEART_HALF:
			heartSprite.setTextureRect(GC::HEART_HALF_TEXTURE_RECT);
			break;

		case GC::HEART_FULL:
			heartSprite.setTextureRect(GC::HEART_FULL_TEXTURE_RECT);
			break;
		}
		
		window.draw(heartSprite);
		heartSprite.move(moveVector);
	}
}

//Came from help from the SFML discord server, centres text origin
void CentreTextYOrigin(sf::Text& text)
{
	const auto localBounds = sf::Vector2f(0.f, text.getLocalBounds().top);
	const auto globalOrigin = sf::Vector2f(0.f, text.getGlobalBounds().height / 2.0f);

	text.setOrigin(localBounds + globalOrigin);
}

//Returns the top left corner of the current view
Dim2Df GetGameUIPosition(const GameData& game, const bool& timeUI)
{
	Dim2Df position;

	if (timeUI)
	{
		position = { game.playerPosition.x + (game.cameraDimensions.x / 2) - (GC::UI_BORDER * 3), game.playerPosition.y - (game.cameraDimensions.y / 2) + GC::UI_BORDER };
	}
	else
	{
		position = { game.playerPosition.x - (game.cameraDimensions.x / 2) + GC::UI_BORDER, game.playerPosition.y - (game.cameraDimensions.y / 2) + GC::UI_BORDER };
	}

	return position;
}

void UI::Init(const GameData& game)
{
	//Sprites
	heartSprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);
	heartSprite.setTextureRect(GC::HEART_FULL_TEXTURE_RECT);
	heartSprite.setOrigin(GC::HEART_ORIGIN);

	coinSprite.setTexture(game.textures[GC::COIN_TEXTURE]);
	coinSprite.setTextureRect({ 0, 0, GC::COINS_DIMENSIONS.x, GC::COINS_DIMENSIONS.y});
	coinSprite.setOrigin(GC::COINS_ORIGIN);

	//Animation
	coinAnim.Init(&GC::COINS_ANIM_DATA);

	//Text
	coinText.setFont(game.font);
	coinText.setCharacterSize(GC::TEXT_CHARACTER_SIZE);
	coinText.setString("0");
	CentreTextYOrigin(coinText);

	timeText.setFont(game.font);
	timeText.setCharacterSize(GC::TEXT_CHARACTER_SIZE);
}

void UI::Render(const GameData& game, sf::RenderWindow &window, const short& health, const short& coins)
{
	coinAnim.UpdateAnimation(coinSprite, game.elapsed);
	RenderHealthBar(game, window, health);
	RenderCoins(game, window, coins);
	RenderTime(game, window);
}

void UI::RenderHealthBar(const GameData& game, sf::RenderWindow& window, const short& health)
{
	//Get health bar position, relative to player position
	heartSprite.setPosition(GetGameUIPosition(game, false));

	//Get sprite states
	const char upgradedMaxHearts = GC::BOOSTED_HEALTH / 2, defaultMaxHearts = GC::PLAYER_HEALTH / 2;
	char hearts[upgradedMaxHearts];
	GetHealthBarState(hearts, health);

	//Draw health bar
	if (game.metrics.purchasedHealthMax)
	{
		RenderHearts(window, heartSprite, hearts, upgradedMaxHearts);
	}
	else
	{
		RenderHearts(window, heartSprite, hearts, defaultMaxHearts);
	}
}

void UI::RenderCoins(const GameData& game, sf::RenderWindow& window, const short& coins)
{
	Dim2Df position = GetGameUIPosition(game, false);
	position.y += GC::HEART_DIMENSIONS.y + GC::UI_BORDER;

	//Coin sprite
	coinSprite.setPosition(position);

	//Coin Text
	position.x += GC::COINS_DIMENSIONS.x;
	coinText.setString(std::to_string(coins));
	coinText.setPosition(position);

	window.draw(coinSprite);
	window.draw(coinText);
}

void UI::RenderTime(const GameData& game, sf::RenderWindow& window)
{
	Dim2Df position = GetGameUIPosition(game, true);

	//Format current time
	unsigned int time = (unsigned int)floor(game.metrics.totalTime);
	std::string minutes, colon, seconds;
	colon = ":";

	if ((time / 60) < 10)
	{
		minutes = "0" + std::to_string(time / 60);
	}
	else
	{
		minutes = std::to_string(time / 60);
	}

	if ((time % 60) < 10)
	{
		seconds = "0" + std::to_string(time % 60);
	}
	else
	{
		seconds = std::to_string(time % 60);
	}

	//Update text
	timeText.setString(minutes + colon + seconds);
	CentreTextOrigin(timeText);
	timeText.setPosition(position);

	window.draw(timeText);
}