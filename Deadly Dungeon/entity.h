#pragma once
#include "weapon.h"
#include "graphics.h"

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
	//SFML
	sf::Texture* texture;
	sf::Sprite sprite;

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

	//Positioning
	Dim2Df bodyCentre = { 0, 0 }; //Player sprite's body centre
	Dim2Df movementVector = { 0.f, 0.f }; //Current movement values
	Dim2Di frameMovementVector = { 0, 0 }; //Current movement this frame
	sf::IntRect collisionRect = { 0, 0, 0, 0 }; //Collision rect of the sprite

	//Timer
	float invulnerabilityTimer = 0.f; //How long the entity is invulnerable for

	//Structs
	Knockback knock; //Knockback
	DirectionalAngle facing; //Direction the entity is facing
	Weapon weapon; //Weapon the entity is holding
	Animation anim; //Animation of the entity

	//Initiates an attack
	void InitAttack(const GameData& game, const char& attack);

	//Initiates a knockback
	void InitKnockback(const DirectionalAngle& facing, const float& knockPower);

	//Updates knockback
	void UpdateKnockback(const GameData& game);

	//Moves the entity, also updates knockback
	void Move(const GameData& game);

	//Checks if movement is valid, using rectangle intersections
	void CheckMapCollision(const GameData& game, const bool& entityBodyAttack);

	//Renders the entity
	void Render(sf::RenderWindow& window, const GameData& game);

	//Updates any ongoing attacks
	void UpdateAttacks(const GameData& game, std::vector<Projectile>& proj);

	//Update the weapon's state
	void UpdateWeapon(const GameData& game, std::vector<Projectile>& proj);

	//Entity takes damage, returns true if dead
	bool TakeDamage(const unsigned char& damage, const DirectionalAngle& facing, const float& knockPower);

	//Stop entity attack if map collision
	void StopAttackIfMapCollided(const bool& collided);
};

//Game constants
namespace GC
{
	//Entity: General
	const unsigned char FEET_COLLISION_HEIGHT = 2; //Feet collision box height
	const unsigned char C_OFFSET = 1; //Offset for smooth collision
	const unsigned char ENEMY_ATTACK_C_OFFSET = 3; //Offset for better collision
	const float SLOW_MOVEMENT_SPEED = 100.f;
	const float MEDIUM_MOVEMENT_SPEED = 130.f;
	const float FAST_MOVEMENT_SPEED = 160.f;
}