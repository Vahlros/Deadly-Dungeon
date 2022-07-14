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

	//Score page
	int scoreID = 0; //Score page ID, changes which scores are shown
	enum ScoreID { BEST, RECENT, TOTALS };

	//SFML
	sf::Sprite sprite;

	//Changes colour and sets mouseHover bool to true if the mouse is hovering over the button
	void UpdateButtonHover(const Dim2Df& mousePosition, sf::Sound& hoverSound);

	void CheckClick(const Input& input, int& state, bool& startGame, bool& setScores, sf::Sound& selectSound);

	void Render(sf::RenderWindow& window);
};

struct StateManager
{
	//Main stats
	Game game;
	int gameState = 0;
	bool gameStarted = false;
	bool setScores = false;
	Input input;

	//Vectors
	std::vector <sf::Texture> menuTextures;
	std::vector <Menu> menus;
	std::vector <Metrics> scores;
	std::vector <sf::Text> scoresText;

	//Initializes menus and game
	void Init(sf::RenderWindow& window);

	//Menu controls
	void MenusInputHandling(sf::RenderWindow& window);

	//Handles rendering depending on gameState
	void RenderMenus(sf::RenderWindow& window);

	//Queries the database the get scores
	void GetHighScores();

	//Assigns high scores to the scoresText
	void SetHighScores();

	//Renders sprites and text for the score page
	void RenderScores(sf::RenderWindow& window);

	//The core loop for the entire game, menus and gameplay
	void MainLoop(sf::RenderWindow& window);
};

namespace GC //Game constants
{
	//Enums
	enum MENUS { MENU_MAIN, MENU_SCORES, MENU_CONTROLS };

	//Menus: General
	const int NUM_MENUS = 4;
	const int NUM_MENU_TEXTURES = 10;
	const Dim2Df BACK_POSITION = { 1587.f, 36.f };
	//Menus: Main Menu
	const int MAIN_MENU_NUM_BUTTONS = 3;
	const Dim2Df MAIN_PLAY_POSITION = { 1460.f, 611.f };
	const Dim2Df MAIN_SCORES_POSITION = { 1355.f, 769.f };
	const Dim2Df MAIN_CONTROLS_POSITION = { 1222.f, 930.f };
	//Menus: Scores
	const int SCORES_MENU_BUTTONS = 4;
	const int SCORES_MENU_TEXTS = 19;
	const int SCORES_CHARACTER_SIZE = 96;
	const Dim2Df SCORES_RECENT_POSITION = { 76.f, 79.f };
	const Dim2Df SCORES_BEST_POSITION = { 548.f, 79.f };
	const Dim2Df SCORES_TOTALS_POSITION = { 880.f, 79.f };
}