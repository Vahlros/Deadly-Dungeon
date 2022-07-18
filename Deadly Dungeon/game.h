#pragma once
#include "player.h"
#include "ui.h"

struct Game
{
	//Game setup
	GameData data{};
	Player player1{};
	GameUI ui{};
	std::vector<Dim2Df> spawnPoints;
	std::vector<Enemy> enemyList;
	std::vector<Projectile> projectiles;
	std::vector<Room> roomList;

	//Audio
	sf::Sound menuHover;
	sf::Sound menuSelect;

	//SFML
	sf::Clock clock{};

	//Timers
	float enemySpawnTimer = 0.f;

	//Other stuff currently unknown to those who seek to wield it

	//Initializes the game session
	void Init(sf::RenderWindow& window, Input& input);

	//Check player death
	void IsPlayerDead(sf::RenderWindow& window, int& state);

	void ExitGame(sf::RenderWindow& window, int& state, const bool& playerDead);

	//Keeps track of difficulty and enemy spawning
	void EnemyUpdate();

	void EnemyRender(sf::RenderWindow& window);

	//Spawn enemies
	void SpawnEnemies();

	void Update(sf::RenderWindow& window, const Input& input, int& state);
	
	void Render(sf::RenderWindow& window);

	//Resets values to prepare for a new game
	void NewGame(sf::RenderWindow& window);
};

namespace GC //Game constants
{
	//Enums
	enum WAVE_DIFFICULTY_RATINGS { D_TRIVIAL, D_WALK_IN_THE_PARK, D_VERY_EASY, D_EASY, D_KINDA_EASY, D_NORMAL, //Wave difficulty
		D_KINDA_HARD, D_HARD, D_VERY_HARD, D_BRUTAL, D_NIGHTMARE };

	//Shop: General
	const char TOTAL_WATER_SHOPS = 6; //How many water shops are in the game
	const char TOTAL_LAVA_SHOPS = 7; //How many lava shops are in the game

	//Enemy Spawner: General
	const char MAX_ENEMIES = 50; //Maximum number of enemies
	const float ENEMY_SPAWN_TIME = 7.f; //Time until the next wave
	const char MAX_ENEMIES_SPAWNED = 5; //Maximum number of enemies spawned per wave
	const char MAX_SPECIALS_SPAWNED = 3; //Maximum number of powerful enemies spawned per wave
	const float VALID_SPAWN_DISTANCE = GC::TILE_SIZE * 2;
	const short MAX_SPAWN_ATTEMPTS = 10;

	//Game: Win
	const float WIN_TIME = 60.f * 7.f; //Surviving for longer than 6 minutes will play the win music upon death
}