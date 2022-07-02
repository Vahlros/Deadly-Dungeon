#pragma once
//#include "audio.h"
#include "player.h"
#include "rooms.h"
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
};

//Initializes all projectiles
void InitProjectiles(const GameData& game, std::vector<Projectile>& projList);

//Updates all active projectiles
void UpdateProjectiles(const GameData& game, sf::RenderWindow& window, std::vector<Projectile>& projList, std::vector<Enemy>& enemies, Player& player);

//Projectile collision detection
void CheckProjectileCollision(Projectile& proj, std::vector<Enemy>& enemies, Player& player);