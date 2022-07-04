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
	char maxHealth = 0; //Player maximum health
	float speed = 0.f; //Player speed
	float knockbackPower = 100.f; //How fast the knocked enemy is
	float heavyAttackMultiplier = 2.f; //Damage multiplier of heavy attacks
	short coins = 0; //How many coins the player has

	//Operational bools
	bool dodging = false; //If the player is dodging or not
	bool hit = false; //If the player has just been hit

	//Structs
	Entity entity{};

	//Initializes player
	void Init(GameData& game, const Dim2Df& spawnPosition, std::vector<Room>& rooms);

	//Get inputs and react
	void InputHandling(sf::RenderWindow& window, GameData& game, std::vector<Room>& rooms);

	//Controls for keyboard
	void KeyboardControls(const sf::Event& event, GameData& game, std::vector<Room>& rooms);

	//Find movement vector based on booleans
	void KeyboardMovement(const sf::Event& event);

	//Find the entity's facing angle based on mouse position
	void GetMouseAngle(const GameData& game);

	//Controls for gamepad
	void GamepadControls(const sf::Event& event);

	//Checks attack collision against enemies
	void CheckAttackCollision(GameData& game, std::vector<Enemy>& enemies);

	//Updates player state
	void Update(sf::RenderWindow& window, GameData& game, std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies, std::vector<Room>& rooms);

	//Initiates after hit invulnerability
	void UpdateInvulnerability(GameData& game);

	//Player death
	void Dead(GameData& game);

	//Earn money on kill
	void EarnCoins(const char& enemyID);

	//Buy the item of a nearby shop
	void BuyItemFromShop(GameData& game, std::vector<Shop>& shops);

	//Apply item effects
	void ApplyItemEffects(const GameData& game, const char& itemID);
};

namespace GC
{
	//Player: General
	const char BUY_ITEM_RANGE = TILE_SIZE * 2;
	//Player: Upgrades
	const float BOOSTED_SPEED = FAST_MOVEMENT_SPEED; //Upgraded speed
	const float BOOSTED_POWER = 2.f; //Upgraded power
	const float BOOSTED_ATTACK_SPEED = 1.5f; //Upgraded attack speed
	const float BOOSTED_KNOCKBACK_POWER = 160.f; //Upgraded knockback power
	const Dim2Df BIG_WEAPONS_SCALE = {1.1f, 1.3f}; //Upgraded weapon size
}