#pragma once
#include "weapon.h"
#include "graphics.h"
#include "rooms.h"
#include "collision.h"

//Knockback
struct Knockback
{
	//Bools
	bool immovable = false; //If entity is immune to knockback

	//Positioning
	Dim2Df movementVector{ 0.f, 0.f }; //Movement vector of knockback

	//Timer
	float timer = 0.f; //How long the knockback lasts
};

//An interactive creature
struct Entity
{
	//Bools
	bool isPlayer = false; //Player or Enemy
	bool isAlive = true; //Alive or dead
	bool canAttack = true; //Ability to attack
	bool knockback = false; //Is being knocked back
	bool invulnerable = false; //Can be damaged or not
	bool moving = false; //Moving or still
	bool facingRight = true; //Facing right or left
	bool nearPlayer = false; //In the rendered map area or not

	//Main stats
	short health = 0; //Hit points
	float attackSpeed = 1.f; //Attack speed multiplier
	float power = 1.f; //Damage multiplier
	Dim2Df weaponScale = { 1.f, 1.f }; //Weapon scaling

	//Positioning
	Dim2Df bodyCentre = { 0, 0 }; //Player sprite's body centre
	Dim2Df movementVector = { 0.f, 0.f }; //Current movement values
	Dim2Df frameMovementVector = { 0, 0 }; //Current movement this frame
	sf::IntRect collisionRect = { 0, 0, 0, 0 }; //Collision rect of the sprite
	char roomID = 0; //Which room the entity is in
	char uniqueID = -1;

	//Timer
	float invulnerabilityTimer = 0.f; //How long the entity is invulnerable for

	//Structs
	Knockback knock; //Knockback
	DirectionalAngle facing; //Direction the entity is facing
	Weapon weapon; //Weapon the entity is holding
	Animation anim; //Animation of the entity

	//SFML
	sf::Sprite sprite;

	//Audio
	sf::Sound footsteps;
	sf::Sound noise;
	sf::Sound weaponNoise;

	//Initiates an attack
	void InitAttack(const GameData& game, const char& attack);

	//Initiates a knockback
	void InitKnockback(const DirectionalAngle& facing, const float& knockPower);

	//Reduces knockback timer until it disables knockback
	void UpdateKnockback(const GameData& game);

	//Reduces invulnerability timer until it disables invulnerability
	void UpdateInvulnerability(const GameData& game);

	//Moves the entity, also updates knockback
	void Move(const GameData& game);

	//Renders the entity
	void Render(sf::RenderWindow& window, const GameData& game);

	//Updates any ongoing attacks
	void UpdateAttacks(const GameData& game, std::vector<Projectile>& proj);

	//Update the weapon's state
	void UpdateWeapon(const GameData& game, std::vector<Projectile>& proj);

	//Entity takes damage, returns true if dead
	bool TakeDamage(const unsigned char& damage, const DirectionalAngle& facing, const float& knockPower);

	//Find current room
	void FindCurrentRoom(std::vector<Room>& rooms);

	//Plays a hit noise, replaces current noise
	void HitNoise(const GameData& game, const int& ID);
};

//Game constants
namespace GC
{
	//Entity: General
	const float SLOW_MOVEMENT_SPEED = 100.f;
	const float MEDIUM_MOVEMENT_SPEED = 130.f;
	const float FAST_MOVEMENT_SPEED = 160.f;
}