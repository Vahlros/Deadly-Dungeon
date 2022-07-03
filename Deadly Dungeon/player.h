#pragma once
#include "enemy.h"
#include "rooms.h"

struct Player
{
	//SFML
	sf::Mouse mouse{}; //Player mouse

	//Positioning
	Dim2Df movementInput = { 0.f, 0.f }; //Player movement inputs

	//Main stats
	float speed = 0.f; //Player speed
	float knockbackPower = 100.f; //How fast the knocked enemy is
	float heavyAttackMultiplier = 2.f; //Damage multiplier of heavy attacks

	//Operational bools
	bool dodging = false; //If the player is dodging or not
	bool hit = false; //If the player has just been hit

	//Structs
	Entity entity{};

	//Initializes player
	void Init(GameData& game, const Dim2Df& spawnPosition);

	//Get inputs and react
	void InputHandling(sf::RenderWindow& window, const GameData& game);

	//Controls for keyboard
	void KeyboardControls(const sf::Event& event, const GameData& game);

	//Find movement vector based on booleans
	void KeyboardMovement(const sf::Event& event);

	//Find the entity's facing angle based on mouse position
	void GetMouseAngle(const GameData& game);

	//Controls for gamepad
	void GamepadControls(const sf::Event& event);

	//Checks attack collision against enemies
	void CheckAttackCollision(std::vector<Enemy>& enemies);

	//Updates player state
	void Update(sf::RenderWindow& window, GameData& game, std::vector<Projectile> projectiles, std::vector<Enemy>& enemies);

	//Initiates after hit invulnerability
	void UpdateInvulnerability(GameData& game);

	//Player death
	void Dead(GameData& game);
};

namespace GC
{
	const char PLAYER_HEALTH = 10; //Player starting health
}