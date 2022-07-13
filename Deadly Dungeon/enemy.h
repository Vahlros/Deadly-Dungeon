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
	bool attackCooldown = true; //If the enemy is in cooldown, spawns in short cooldown
	bool attackCancelImmune = false; //If the enemy is immune to player's attack cancellation
	bool boosted = false; //If the enemy has boosted stats and immunities

	//Operational stats
	float distanceToPlayer = 0.f; //Distance to the player
	float cooldownTimer = GC::SPAWN_ATTACK_COOLDOWN; //Cooldown between attacks

	//Timers
	float differentRoomTimer = 0.f; //How long the enemy has been in a different room
	float checkRoomTimer = 3.f; //Time until next room check
	float attackCancelledCooldown = 2.f; //Time until immunity wears off
	float noiseTimer = 0.f; //Time until next noise chance

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

	//Calculates sound volume based on distance from the player
	void SetSoundVolume();

	//Plays a random noise of the enemy type
	void PlayRandomNoise(const GameData& game);

	//Plays an enemy hit noise
	void HitNoise(const GameData& game);

	//Updates differentRoomTimer and despawns after a certain amount of time in a different room
	void DespawnIfStuck(const GameData& game, const int& playerRoom, std::vector<Room>& rooms);

	//Updates the status of the attack cancellation immunity buff
	void AttackCancellationHandling(const GameData& game);

	//Gives this enemy random boost effects
	void Boost();
};

//Game constants
namespace GC
{
	//Behaviour
	const char RANGE_NERF = 5; //Reduce range before the enemy attacks
	const float CHECK_ROOM_TIMER = 3.f; //How long between room checks
	const float DIFFERENT_ROOM_DESPAWN_TIMER = 29.f; //How long an enemy is stuck in a different room before it despawns
	const float ATTACK_CANCELLATION_COOLDOWN = 5.f; //How long attack cancellation immunity lasts for
	const float KNOCKBACK_RESISTANCE_COOLDOWN = 2.f; //How long knockback immunity lasts for

	//Boosts
	const float ENEMY_BOOST_CHANCE = 30.f; //Percentage chance to boost and enemy, stacks on difficulty over 10 (after 5 minutes)
	const int ENEMY_BOOSTED_HEALTH = 2;
	const float ENEMY_BOOSTED_DAMAGE = 2.f;
}