#include "menu.h"

void StateManager::Init(sf::RenderWindow& window)
{
	game.Init(window, input);

	//Menu textures
	menuTextures.resize(GC::NUM_MENU_TEXTURES);
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
	menuTextures[7].create(231, 81);
	menuTextures[7].loadFromFile("graphics/best button.png");
	menuTextures[8].create(356, 84);
	menuTextures[8].loadFromFile("graphics/recent button.png");
	menuTextures[9].create(351,  81);
	menuTextures[9].loadFromFile("graphics/totals button.png");

	//Menus
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

	menus[1].buttons.resize(GC::SCORES_MENU_BUTTONS);
	menus[1].buttons[0].ID = GC::B_BACK;
	menus[1].buttons[0].sprite.setTexture(menuTextures[4]);
	menus[1].buttons[0].sprite.setPosition(GC::BACK_POSITION);
	menus[1].buttons[1].ID = GC::B_SCORES_BEST;
	menus[1].buttons[1].sprite.setTexture(menuTextures[7]);
	menus[1].buttons[1].sprite.setPosition(GC::SCORES_BEST_POSITION);
	menus[1].buttons[2].ID = GC::B_SCORES_RECENT;
	menus[1].buttons[2].sprite.setTexture(menuTextures[8]);
	menus[1].buttons[2].sprite.setPosition(GC::SCORES_RECENT_POSITION);
	menus[1].buttons[3].ID = GC::B_SCORES_TOTALS;
	menus[1].buttons[3].sprite.setTexture(menuTextures[9]);
	menus[1].buttons[3].sprite.setPosition(GC::SCORES_TOTALS_POSITION);

	//Controls
	menus[2].ID = GC::MENU_CONTROLS;
	menus[2].fromID = GC::MENU_MAIN;
	menus[2].sprite.setTexture(menuTextures[6]);
	menus[2].sprite.setPosition(0.f, 0.f);

	menus[2].buttons.resize(1);
	menus[2].buttons[0].ID = GC::B_BACK;
	menus[2].buttons[0].sprite.setTexture(menuTextures[4]);
	menus[2].buttons[0].sprite.setPosition(GC::BACK_POSITION);

	//Score page specifics
	scores.resize(GC::SCORES_MENU_BUTTONS - 1); //Minus back button
	scoresText.resize(GC::SCORES_MENU_TEXTS);

	for (int i = 0; i < GC::SCORES_MENU_TEXTS; i++)
	{
		scoresText[i].setFont(game.data.font);
		scoresText[i].setFillColor(GC::FONT_COLOUR);
		scoresText[i].setCharacterSize(GC::SCORES_CHARACTER_SIZE);
		scoresText[i].setString("0");
		CentreTextYOrigin(scoresText[i]);
	}

	//Enemy Kills
	scoresText[0].setPosition(559, 351);
	scoresText[1].setPosition(559, 414);
	scoresText[2].setPosition(559, 478);
	scoresText[3].setPosition(559, 547);
	scoresText[4].setPosition(559, 612);
	//Weapon Kills
	scoresText[5].setPosition(559, 802);
	scoresText[6].setPosition(559, 865);
	scoresText[7].setPosition(559, 929);
	scoresText[8].setPosition(559, 992);
	//Damage Received
	scoresText[9].setPosition(814, 351);
	scoresText[10].setPosition(814, 415);
	scoresText[11].setPosition(814, 479);
	scoresText[12].setPosition(814, 543);
	//Survival
	scoresText[13].setPosition(1006, 736);
	scoresText[14].setPosition(1006, 795);
	//Coins
	scoresText[15].setPosition(1390, 479);
	scoresText[16].setPosition(1390, 543);
	//Shop
	scoresText[17].setPosition(1390, 927);
	scoresText[18].setPosition(1390, 993);

	//Retrieve scores from database
	GetHighScores();
}

void StateManager::MenusInputHandling(sf::RenderWindow& window)
{
	if (input.escapePressed)
	{
		window.close();
		game.data.metrics.database.Close();
	}

	menus[gameState].UpdateButtonHover(input.mousePosition, game.menuHover);

	switch (gameState)
	{
	case GC::STATE_MAIN_MENU:
		menus[0].CheckClick(input, gameState, gameStarted, setScores, game.menuSelect);
		break;

	case GC::STATE_SCORES:
		menus[1].CheckClick(input, gameState, gameStarted, setScores, game.menuSelect);
		break;

	case GC::STATE_TUTORIAL:
		menus[2].CheckClick(input, gameState, gameStarted, setScores, game.menuSelect);
		break;
	}

	if (setScores)
	{
		SetHighScores();
		setScores = false;
	}

	//Audio
	if (game.data.music.getStatus() != sf::Music::Playing)
	{
		PlayMusic(game.data.music, GC::MUSIC_MENUS, true);
	}

	if (gameStarted)
	{
		PlayMusic(game.data.music, GC::MUSIC_GAME, true);
		game.NewGame(window);
		gameStarted = false;
	}
}

void StateManager::RenderMenus(sf::RenderWindow& window)
{
	switch (gameState)
	{
	case GC::STATE_MAIN_MENU:
		menus[0].Render(window);
		break;

	case GC::STATE_SCORES:
		menus[1].Render(window);
		RenderScores(window);
		break;

	case GC::STATE_TUTORIAL:
		menus[2].Render(window);
		break;
	}
}

Metrics GetScores(const int& type)
{
	//Get best game ID
	int id = 0;
	bool exists = true;
	std::string sqlstr;
	Metrics met{};
	MyDB db;
	db.Init("scores.db", exists);

	if (exists)
	{
		switch (type)
		{
		case Menu::ScoreID::BEST:
			db.ExecQuery("SELECT * FROM MainStats ORDER BY Time DESC LIMIT 1");
			id = db.GetInt(0, "ID");
			break;

		case Menu::ScoreID::RECENT:
			db.ExecQuery("SELECT * FROM MainStats ORDER BY ID DESC LIMIT 1");
			id = db.GetInt(0, "ID");
			break;

		case Menu::ScoreID::TOTALS:
			db.ExecQuery("SELECT SUM(Time) AS 'Time', SUM(Kills) AS 'Kills', SUM(CoinsEarned) AS 'CoinsEarned' FROM MainStats");
			break;
		}
		
		//MainStats
		met.totalTime = static_cast<float>(db.GetInt(0, "Time"));
		met.enemiesKilled = db.GetInt(0, "Kills");
		met.coinsEarned = db.GetInt(0, "CoinsEarned");

		//Kills
		if (type == Menu::ScoreID::TOTALS)
		{
			db.ExecQuery("SELECT SUM(Imp) AS 'Imp', SUM(LesserDemon) AS 'LesserDemon', " \
				"SUM(Aberrant) AS 'Aberrant', SUM(GreaterDemon) AS 'GreaterDemon' FROM Kills");
		}
		else
		{
			db.ExecQuery("SELECT * FROM Kills WHERE ID = " + std::to_string(id));
		}

		met.impKills = db.GetInt(0, "Imp");
		met.lesserDemonKills = db.GetInt(0, "LesserDemon");
		met.aberrantKills = db.GetInt(0, "Aberrant");
		met.greaterDemonKills = db.GetInt(0, "GreaterDemon");

		//WeaponKills
		if (type == Menu::ScoreID::TOTALS)
		{
			db.ExecQuery("SELECT SUM(Sword) AS 'Sword', SUM(FancySword) AS 'FancySword', " \
				"SUM(Spear) AS 'Spear', SUM(Projectiles) AS 'Projectiles' FROM WeaponKills");
		}
		else
		{
			db.ExecQuery("SELECT * FROM WeaponKills WHERE ID = " + std::to_string(id));
		}

		met.swordKills = db.GetInt(0, "Sword");
		met.fancySwordKills = db.GetInt(0, "FancySword");
		met.spearKills = db.GetInt(0, "Spear");
		met.projectileKills = db.GetInt(0, "Projectiles");

		//DamageTaken
		if (type == Menu::ScoreID::TOTALS)
		{
			db.ExecQuery("SELECT SUM(Imp) AS 'Imp', SUM(LesserDemon) AS 'LesserDemon', " \
				"SUM(Aberrant) AS 'Aberrant', SUM(GreaterDemon) AS 'GreaterDemon' FROM DamageTaken");
		}
		else
		{
			db.ExecQuery("SELECT * FROM DamageTaken WHERE ID = " + std::to_string(id));
		}

		met.impPlayerDamage = db.GetInt(0, "Imp");
		met.lesserDemonPlayerDamage = db.GetInt(0, "LesserDemon");
		met.aberrantPlayerDamage = db.GetInt(0, "Aberrant");
		met.greaterDemonPlayerDamage = db.GetInt(0, "GreaterDemon");

		//Shop
		if (type == Menu::ScoreID::TOTALS)
		{
			db.ExecQuery("SELECT SUM(ItemsPurchased) AS 'ItemsPurchased', SUM(HealsPurchased) AS 'HealsPurchased', " \
				"SUM(CoinsSpent) AS 'CoinsSpent' FROM Shop");
		}
		else
		{
			db.ExecQuery("SELECT * FROM Shop WHERE ID = " + std::to_string(id));
		}
		
		met.itemsPurchased = db.GetInt(0, "ItemsPurchased");
		met.healsPurchased = db.GetInt(0, "HealsPurchased");
		met.coinsSpent = db.GetInt(0, "CoinsSpent");
	}

	db.Close();

	return met;
}

void StateManager::GetHighScores()
{
	scores[Menu::ScoreID::BEST] = GetScores(Menu::ScoreID::BEST);
	scores[Menu::ScoreID::RECENT] = GetScores(Menu::ScoreID::RECENT);
	scores[Menu::ScoreID::TOTALS] = GetScores(Menu::ScoreID::TOTALS);
}

void StateManager::SetHighScores()
{
	int page = menus[GC::MENU_SCORES].scoreID;

	//Enemy Kills
	scoresText[0].setString(std::to_string(scores[page].impKills));
	scoresText[1].setString(std::to_string(scores[page].lesserDemonKills));
	scoresText[2].setString(std::to_string(scores[page].aberrantKills));
	scoresText[3].setString(std::to_string(scores[page].greaterDemonKills));
	scoresText[4].setString(std::to_string(scores[page].enemiesKilled));
	//Weapon Kills
	scoresText[5].setString(std::to_string(scores[page].swordKills));
	scoresText[6].setString(std::to_string(scores[page].fancySwordKills));
	scoresText[7].setString(std::to_string(scores[page].spearKills));
	scoresText[8].setString(std::to_string(scores[page].projectileKills));
	//Damage Received
	scoresText[9].setString(std::to_string(scores[page].impPlayerDamage));
	scoresText[10].setString(std::to_string(scores[page].lesserDemonPlayerDamage));
	scoresText[11].setString(std::to_string(scores[page].aberrantPlayerDamage));
	scoresText[12].setString(std::to_string(scores[page].greaterDemonPlayerDamage));
	//Survival
	int totalTime = static_cast<int>(scores[page].totalTime);
	scoresText[13].setString(std::to_string(totalTime));
	scoresText[14].setString(std::to_string(totalTime / static_cast<int>(GC::ENEMY_SPAWN_TIME)));
	//Coins
	scoresText[15].setString(std::to_string(scores[page].coinsEarned));
	scoresText[16].setString(std::to_string(scores[page].coinsSpent));
	//Shop
	scoresText[17].setString(std::to_string(scores[page].itemsPurchased));
	scoresText[18].setString(std::to_string(scores[page].healsPurchased));

	//Recentre text
	for (int i = 0; i < GC::SCORES_MENU_TEXTS; i++)
	{
		CentreTextYOrigin(scoresText[i]);
	}
}

void StateManager::RenderScores(sf::RenderWindow& window)
{
	for (int i = 0; i < GC::SCORES_MENU_TEXTS; i++)
	{
		window.draw(scoresText[i]);
	}
}

void StateManager::MainLoop(sf::RenderWindow& window)
{
	while (window.isOpen())
	{
		input.Clear();
		input.ProcessEvents(window);

		if (gameState < GC::STATE_PLAYING) //In menus
		{
			MenusInputHandling(window);
		}
		else
		{
			game.Update(window, input, gameState);
		}

		if (game.data.playerDead)
		{
			//Refresh high scores
			GetHighScores();
			SetHighScores();
			game.data.playerDead = false;
		}

		window.clear(sf::Color::Black);

		if (gameState < GC::STATE_PLAYING) //In menus
		{
			RenderMenus(window);
		}
		else
		{
			game.Render(window);
		}

		window.display();
	}
}

//Alters current game state
void ButtonPress(int& state, const Button& button, const int& fromID, bool& startGame, int& scoreID, bool& setScores)
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
		scoreID = Menu::ScoreID::RECENT;
		setScores = true;
		break;

	case GC::B_TUTORIAL:
		state = GC::STATE_TUTORIAL;
		break;

	case GC::B_SCORES_BEST:
		scoreID = Menu::ScoreID::BEST;
		setScores = true;
		break;

	case GC::B_SCORES_RECENT:
		scoreID = Menu::ScoreID::RECENT;
		setScores = true;
		break;

	case GC::B_SCORES_TOTALS:
		scoreID = Menu::ScoreID::TOTALS;
		setScores = true;
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

void Menu::CheckClick(const Input& input, int& state, bool& startGame, bool& setScores, sf::Sound& selectSound)
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].mouseHover && input.leftClickPressed)
		{
			ButtonPress(state, buttons[i], fromID, startGame, scoreID, setScores);
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