#include "menu.h"

void InitializeMenus(std::vector <sf::Texture>& menuTextures, std::vector <Menu>& menus)
{
	menuTextures.resize(GC::NUM_MENU_TEXTURES);

	//Textures
	menuTextures[0].create(1920, 1080);
	menuTextures[0].loadFromFile("main menu background.png");
	menuTextures[1].create(252, 80);
	menuTextures[1].loadFromFile("play button.png");
	menuTextures[2].create(352, 86);
	menuTextures[2].loadFromFile("scores button.png");
	menuTextures[3].create(486, 84);
	menuTextures[3].loadFromFile("tutorial button.png");
	menuTextures[4].create(252, 83);
	menuTextures[4].loadFromFile("back button.png");

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
	menus[0].buttons[2].sprite.setPosition(GC::MAIN_TUTORIAL_POSITION);

	//Scores


	//Tutorial


}

void ButtonPress(int& state, const Button& button, const int& fromID, bool& startGame)
{
	switch (button.ID)
	{
	case GC::B_BACK:
		state = fromID;
		break;

	case GC::B_PLAY:
		state = GC::S_PLAYING;
		startGame = true;
		break;

	case GC::B_SCORES:
		state = GC::S_SCORES;
		break;

	case GC::B_TUTORIAL:
		state = GC::S_TUTORIAL;
		break;
	}
}

void MenusInputHandling(sf::RenderWindow& window, int& state, const Input& input, std::vector<Menu>& menus, Game& game, bool& startGame)
{
	if (input.escapePressed)
	{
		window.close();
	}

	menus[state].UpdateButtonHover(input.mousePosition);

	switch (state)
	{
	case GC::S_MAIN_MENU:
		menus[0].CheckClick(input, state, startGame);
		break;

	case GC::S_SCORES:
		menus[1].CheckClick(input, state, startGame);
		break;

	case GC::S_TUTORIAL:
		menus[2].CheckClick(input, state, startGame);
		break;
	}
}

void RenderMenus(sf::RenderWindow& window, std::vector<Menu>& menus, int& state)
{
	switch (state)
	{
	case GC::S_MAIN_MENU:
		menus[0].Render(window);
		break;

	case GC::S_SCORES:
		menus[1].Render(window);
		break;

	case GC::S_TUTORIAL:
		menus[2].Render(window);
		break;
	}
}

void Menu::UpdateButtonHover(const Dim2Df& mousePosition)
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].sprite.getGlobalBounds().contains(mousePosition))
		{
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

void Menu::CheckClick(const Input& input, int& state, bool& startGame)
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].mouseHover && input.leftClickPressed)
		{
			ButtonPress(state, buttons[i], fromID, startGame);
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