#include "menu.h"

void InitializeMenus(std::vector <sf::Texture>& menuTextures, std::vector <Menu>& menus)
{
	menuTextures.resize(GC::NUM_MENU_TEXTURES);

	//Textures
	menuTextures[0].create(1920, 1080);
	menuTextures[0].loadFromFile("graphics/main menu.png");
	menuTextures[1].create(252, 80);
	menuTextures[1].loadFromFile("graphics/play button.png");
	menuTextures[2].create(352, 86);
	menuTextures[2].loadFromFile("graphics/scores button.png");
	menuTextures[3].create(486, 84);
	menuTextures[3].loadFromFile("graphics/controls button.png");
	menuTextures[4].create(252, 83);
	menuTextures[4].loadFromFile("graphics/back button.png");
	menuTextures[5].create(1920, 1080);
	menuTextures[5].loadFromFile("graphics/scores menu.png");
	menuTextures[6].create(1920, 1080);
	menuTextures[6].loadFromFile("graphics/controls menu.png");

	menus.resize(GC::NUM_MENUS);

	//Main menu
	menus[0].ID = GC::MENU_MAIN;
	menus[0].sprite.setTexture(menuTextures[0]);
	menus[0].sprite.setPosition(0.f, 0.f);

	menus[0].buttons.resize(GC::MAIN_MENU_NUM_BUTTONS);
	menus[0].buttons[0].ID = GC::B_PLAY;
	menus[0].buttons[0].sprite.setTexture(menuTextures[1]);
	menus[0].buttons[0].sprite.setPosition(GC::MAIN_PLAY_POSITION);
	menus[0].buttons[1].ID = GC::B_SCORES;
	menus[0].buttons[1].sprite.setTexture(menuTextures[2]);
	menus[0].buttons[1].sprite.setPosition(GC::MAIN_SCORES_POSITION);
	menus[0].buttons[2].ID = GC::B_TUTORIAL;
	menus[0].buttons[2].sprite.setTexture(menuTextures[3]);
	menus[0].buttons[2].sprite.setPosition(GC::MAIN_CONTROLS_POSITION);

	//Scores
	menus[1].ID = GC::MENU_SCORES;
	menus[1].fromID = GC::MENU_MAIN;
	menus[1].sprite.setTexture(menuTextures[5]);
	menus[1].sprite.setPosition(0.f, 0.f);

	menus[1].buttons.resize(1);
	menus[1].buttons[0].ID = GC::B_BACK;
	menus[1].buttons[0].sprite.setTexture(menuTextures[4]);
	menus[1].buttons[0].sprite.setPosition(GC::BACK_POSITION);

	//Controls
	menus[2].ID = GC::MENU_CONTROLS;
	menus[2].fromID = GC::MENU_MAIN;
	menus[2].sprite.setTexture(menuTextures[6]);
	menus[2].sprite.setPosition(0.f, 0.f);

	menus[2].buttons.resize(1);
	menus[2].buttons[0].ID = GC::B_BACK;
	menus[2].buttons[0].sprite.setTexture(menuTextures[4]);
	menus[2].buttons[0].sprite.setPosition(GC::BACK_POSITION);
}

void ButtonPress(int& state, const Button& button, const int& fromID, bool& startGame)
{
	switch (button.ID)
	{
	case GC::B_BACK:
		state = fromID;
		break;

	case GC::B_PLAY:
		state = GC::STATE_PLAYING;
		startGame = true;
		break;

	case GC::B_SCORES:
		state = GC::STATE_SCORES;
		break;

	case GC::B_TUTORIAL:
		state = GC::STATE_TUTORIAL;
		break;
	}
}

void MenusInputHandling(sf::RenderWindow& window, int& state, const Input& input, std::vector<Menu>& menus, Game& game, bool& startGame)
{
	if (input.escapePressed)
	{
		window.close();
	}

	menus[state].UpdateButtonHover(input.mousePosition, game.menuHover);

	switch (state)
	{
	case GC::STATE_MAIN_MENU:
		menus[0].CheckClick(input, state, startGame, game.menuSelect);
		break;

	case GC::STATE_SCORES:
		menus[1].CheckClick(input, state, startGame, game.menuSelect);
		break;

	case GC::STATE_TUTORIAL:
		menus[2].CheckClick(input, state, startGame, game.menuSelect);
		break;
	}

	//Audio
	if (game.data.music.getStatus() != sf::Music::Playing)
	{
		PlayMusic(game.data.music, GC::MUSIC_MENUS, true);
	}

	if (startGame)
	{
		PlayMusic(game.data.music, GC::MUSIC_GAME, true);
	}
}

void RenderMenus(sf::RenderWindow& window, std::vector<Menu>& menus, int& state)
{
	switch (state)
	{
	case GC::STATE_MAIN_MENU:
		menus[0].Render(window);
		break;

	case GC::STATE_SCORES:
		menus[1].Render(window);
		break;

	case GC::STATE_TUTORIAL:
		menus[2].Render(window);
		break;
	}
}

void Menu::UpdateButtonHover(const Dim2Df& mousePosition, sf::Sound& hoverSound)
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].sprite.getGlobalBounds().contains(mousePosition))
		{
			if (!buttons[i].mouseHover)
			{
				hoverSound.play();
			}

			buttons[i].mouseHover = true;
			buttons[i].sprite.setColor(GC::PLAYER_COLOUR);
		}
		else
		{
			buttons[i].mouseHover = false;
			buttons[i].sprite.setColor(sf::Color::White);
		}
	}
}

void Menu::CheckClick(const Input& input, int& state, bool& startGame, sf::Sound& selectSound)
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].mouseHover && input.leftClickPressed)
		{
			ButtonPress(state, buttons[i], fromID, startGame);
			selectSound.play();
		}
	}
}

void Menu::Render(sf::RenderWindow& window)
{
	window.draw(sprite);

	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		window.draw(buttons[i].sprite);
	}
}