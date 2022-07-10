#pragma once
#include "entity.h"

struct Enemy
{
	//Stuff unkown but that will soon come to be
	
	//Setup stats
	char ID = 0; //Enemy ID

	//Operational bools
	bool active = false; //If the enemy has been spawned in and not died yet
	bool useAttackOne = true; //Determines which attack to use
	bool madman = false; //If the enemy will wildly attack
	bool attackCooldown = false; //If the enemy is in cooldown
	bool attackCancelImmune = false; //If the enemy is immune to player's attack cancellation

	//Operational stats
	float distanceToPlayer = 0.f; //Distance to the player
	float cooldownTimer = GC::ATTACK_COOLDOWN; //Cooldown between attacks

	//Timers
	float differentRoomTimer = 0.f; //How long the enemy has been in a different room
	float checkRoomTimer = 3.f; //Time until next room check
	float attackCancelledCooldown = 2.f; //Time until immunity wears off

	//Structs
	Entity entity;

	//Initialize enemy
	void Init(GameData& game, const Dim2Df& spawnPosition, const char& uID);

	//Face enemy towards player
	void TargetPlayer(const Entity& playerEntity);

	//Move enemy towards player
	void MoveTowardsPlayer(const GameData& game);
	
	//If in range, attack
	void CheckAttackRange(const GameData& game);

	//Enemy behaviour
	void Update(GameData& game, std::vector<Projectile>& proj, std::vector<Room>& rooms, Entity& playerEntity);

	//Check if attack hits player
	void CheckAttackCollision(GameData& game, Entity& playerEntity);

	//Handles attack cancellation immunity
	void AttackCancel();
};

//Game constants
namespace GC
{
	//Behaviour
	const char RANGE_NERF = 5; //Reduce range before the enemy attacks
	const float CHECK_ROOM_TIMER = 3.f; //How long between room checks
	const float DIFFERENT_ROOM_DESPAWN_TIMER = 29.f; //How long an enemy is stuck in a different room before it despawns
	const float ATTACK_CANCELLATION_COOLDOWN = 5.f; //How long cancellation immunity lasts for
	const float KNOCKBACK_RESISTANCE_COOLDOWN = 2.f; //How long cancellation immunity lasts for
}