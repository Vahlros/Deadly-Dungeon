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

void UI::Init(const GameData& game)
{
	//Sprites
	heartSprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);
	heartSprite.setTextureRect(GC::HEART_FULL_TEXTURE_RECT);
}

void UI::Render(const GameData& game, sf::RenderWindow &window, const short& health, const short& coins)
{
	RenderHealthBar(game, window, health);
	//RenderCoins(window, coins);
}

void UI::RenderHealthBar(const GameData& game, sf::RenderWindow& window, const short& health)
{
	//Get health bar position, relative to player position
	Dim2Df position = { game.playerPosition.x - (game.cameraDimensions.x / 2) + GC::UI_BORDER, game.playerPosition.y - (game.cameraDimensions.y / 2) + GC::UI_BORDER };
	heartSprite.setPosition(position);

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