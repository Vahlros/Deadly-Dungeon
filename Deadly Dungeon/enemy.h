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

	//Operational stats
	float distanceToPlayer = 0.f; //Distance to the player
	float cooldownTimer = GC::ATTACK_COOLDOWN; //Cooldown between attacks

	//Positioning

	//Structs
	Entity entity{};

	//Initialize enemy
	void Init(GameData& game, const Dim2Df spawnPosition);

	//Face enemy towards player
	void TargetPlayer(const Entity& playerEntity);

	//Move enemy towards player
	void MoveTowardsPlayer(const GameData& game);
	
	//If in range, attack
	void CheckAttackRange(const GameData& game);

	//Enemy behaviour
	void Update(GameData& game, std::vector<Projectile>& proj, Entity& playerEntity);

	//Check if attack hits player
	void CheckAttackCollision(GameData& game, Entity& playerEntity);
};

//Game constants
namespace GC
{
	//Behaviour
	const char RANGE_NERF = 5; //Reduce range before the enemy attacks
}