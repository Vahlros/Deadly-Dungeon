#pragma once
//#include "audio.h"
#include "player.h"
#include "ui.h"

struct Game
{
	//Game setup
	GameData data{};
	Player player1;
	std::vector<Dim2Df> spawnPoints;
	std::vector<Enemy> enemyList;
	std::vector<Projectile> projectiles;
	std::vector<Room> roomList;

	//SFML
	sf::Clock clock;

	//Other stuff currently unknown to those who seek to wield it

	//Initializes the game session
	void Init(sf::RenderWindow& window);

	//Main game loop
	void GameLoop(sf::RenderWindow& window);

	//Check player death
	void IsPlayerDead();
};

namespace GC //Game constants
{
	//Shop: General
	const char TOTAL_WATER_SHOPS = 6; //How many water shops are in the game
	const char TOTAL_LAVA_SHOPS = 7; //How many lava shops are in the game
}

//Initializes all projectiles
void InitProjectiles(const GameData& game, std::vector<Projectile>& projList);

//Updates all active projectiles
void UpdateProjectiles(const GameData& game, sf::RenderWindow& window, std::vector<Projectile>& projList, std::vector<Enemy>& enemies, Player& player);

//Projectile collision detection
void CheckProjectileCollision(Projectile& proj, std::vector<Enemy>& enemies, Player& player);

//Initializes shops
void InitShops(const GameData& game, std::vector<Room>& rooms);

//Render shops and fountains near the player
void RenderMapNearPlayer(const GameData& game, sf::RenderWindow& window, std::vector<Room>& rooms, sf::Sprite playerSprite);

//Find a random spawn position
Dim2Df GetRandomSpawn(std::vector<Room>& rooms, const bool& randRoom, const char& roomID);