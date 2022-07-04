#include "enemy.h"
#include "maths.h"

//Initialize enemy
void Enemy::Init(GameData& game, const Dim2Df& spawnPosition, const char& uID)
{
	entity.uniqueID = uID;
	active = true;
	checkRoomTimer = GC::CHECK_ROOM_TIMER;

	switch (ID)
	{
	case GC::ID_IMP:
		//Entity
		entity.health = GC::IMP_HEALTH;
		entity.collisionRect = GC::IMP_BODY_RECT;

		//SFML
		entity.sprite.setTexture(game.textures[GC::IMP_TEXTURE]);
		entity.sprite.setTextureRect({ 0, 0, GC::IMP_DIMENSIONS.x, GC::IMP_DIMENSIONS.y });
		entity.sprite.setOrigin(GC::IMP_BODY_CENTRE);

		//Weapon
		entity.weapon = GC::IMP_WEAPON;
		break;

	case GC::ID_LESSER_DEMON:
		//Entity
		entity.health = GC::L_DEMON_HEALTH;
		entity.collisionRect = GC::L_DEMON_BODY_RECT;

		//SFML
		entity.sprite.setTexture(game.textures[GC::L_DEMON_TEXTURE]);
		entity.sprite.setTextureRect({ 0, 0, GC::L_DEMON_DIMENSIONS.x, GC::L_DEMON_DIMENSIONS.y });
		entity.sprite.setOrigin(GC::L_DEMON_BODY_CENTRE);

		//Weapon
		entity.weapon = GC::LESSER_DEMON_WEAPON;
		break;

	case GC::ID_ABERRANT:
		//Entity
		entity.health = GC::ABERRANT_HEALTH;
		entity.collisionRect = GC::ABERRANT_BODY_RECT;

		//SFML
		entity.sprite.setTexture(game.textures[GC::ABERRANT_TEXTURE]);
		entity.sprite.setTextureRect({ 0, 0, GC::ABERRANT_DIMENSIONS.x, GC::ABERRANT_DIMENSIONS.y });
		entity.sprite.setOrigin(GC::ABERRANT_BODY_CENTRE);

		//Weapon
		entity.weapon = GC::RUSTED_SWORD;
		break;

	case GC::ID_GREATER_DEMON:
		//Entity
		entity.health = GC::G_DEMON_HEALTH;
		entity.collisionRect = GC::G_DEMON_BODY_RECT;

		//SFML
		entity.sprite.setTexture(game.textures[GC::G_DEMON_TEXTURE]);
		entity.sprite.setTextureRect({ 0, 0, GC::G_DEMON_DIMENSIONS.x, GC::G_DEMON_DIMENSIONS.y });
		entity.sprite.setOrigin(GC::G_DEMON_BODY_CENTRE);

		//Weapon
		entity.weapon = GC::GREATER_DEMON_WEAPON;
		break;

	default:
		printf("Enemy ID not recognised, enemy not initialized properly");
	}

	entity.sprite.setPosition(spawnPosition);
	entity.weapon.Init(game, entity.isPlayer, entity.anim);
	entity.weapon.attack0.enemyID = ID;
	entity.weapon.attack1.enemyID = ID;
	entity.anim.Init(&GC::ENEMY_ANIM_IDLE);
}

//Face enemy towards player
void Enemy::TargetPlayer(const Entity& playerEntity)
{
	//Find origin and target points
	Dim2Df centre = entity.sprite.getPosition();
	Dim2Df targetPosition = playerEntity.sprite.getPosition();

	//Get vector between points, then calculate directional angle using vector
	if (centre == targetPosition)
	{
		entity.facing = DirectionalAngle{};
	}
	else
	{
		entity.facing = CalculateDirectionalAngleFromVector(targetPosition - centre);
	}
}

//Move enemy towards player
void Enemy::MoveTowardsPlayer(const GameData& game)
{
	float speed = 0.f;

	switch (ID)
	{
	case GC::ID_IMP:
		speed = GC::SLOW_MOVEMENT_SPEED;
		break;

	case GC::ID_LESSER_DEMON:
		speed = GC::SLOW_MOVEMENT_SPEED;
		break;

	case GC::ID_ABERRANT:
		speed = GC::SLOW_MOVEMENT_SPEED;
		break;

	case GC::ID_GREATER_DEMON:
		speed = GC::SLOW_MOVEMENT_SPEED;
		break;
	}

	entity.movementVector = CalculateVectorOfMagnitude(entity.facing, speed);

	if (entity.knockback)
	{
		entity.Move(game);
	}
	else if (distanceToPlayer > GC::TILE_SIZE)
	{
		entity.Move(game);

		if (!entity.moving)
		{
			entity.moving = true;
			entity.anim.Init(&GC::ENEMY_ANIM_MOVE);
		}
	}
	else
	{
		if (entity.moving)
		{
			entity.moving = false;
			entity.anim.Init(&GC::ENEMY_ANIM_IDLE);
		}
	}
}

//If in range: Attack!
void Enemy::CheckAttackRange(const GameData& game)
{
	//Checks range and initiates attack
	if (useAttackOne)
	{
		if (distanceToPlayer < (entity.weapon.attack0Range - GC::RANGE_NERF))
		{
			entity.moving = false;
			entity.InitAttack(game, GC::FIRST_ATTACK);
			entity.weapon.attack0.attackRelease = true;

			//Attack variation
			if (entity.weapon.hasTwoAttacks)
			{
				useAttackOne = false;
			}
		}
	} 
	else
	{
		if (distanceToPlayer < (entity.weapon.attack1Range - GC::RANGE_NERF))
		{
			entity.moving = false;
			entity.InitAttack(game, GC::SECOND_ATTACK);
			entity.weapon.attack1.attackRelease = true;
			useAttackOne = true;
		}
	}
}

//Enemy behaviour
void Enemy::Update(GameData& game, std::vector<Projectile>& proj, std::vector<Room>& rooms, Entity& playerEntity)
{
	if (entity.isAlive)
	{
		//Target and movement
		TargetPlayer(playerEntity);
		distanceToPlayer = CalculateMagnitudeOfVector(playerEntity.sprite.getPosition() - entity.sprite.getPosition());

		//Attack handling
		if (entity.canAttack && !attackCooldown)
		{
			MoveTowardsPlayer(game);
			CheckAttackRange(game);
			entity.UpdateWeapon(game, proj); //Updates held weapons
		}
		else if (entity.weapon.attacking)
		{
			entity.UpdateWeapon(game, proj); //Updates attack

			if (entity.canAttack) //Applies cooldown as soon as an attack ends
			{
				attackCooldown = true;
			}
			else
			{
				CheckAttackCollision(game, playerEntity);
			}

			if (entity.weapon.visible) //Aberrants always chase the player
			{
				MoveTowardsPlayer(game);
			}
		}
		else //(attackCooldown)
		{
			MoveTowardsPlayer(game);
			entity.UpdateWeapon(game, proj); //Updates held weapons
			cooldownTimer -= game.elapsed;

			if (cooldownTimer < GC::ZERO)
			{
				attackCooldown = false;
				cooldownTimer = GC::ATTACK_COOLDOWN;
			}
		}

		//Check for stuck enemies, despawn if stuck for too long
		checkRoomTimer -= game.elapsed;

		if (checkRoomTimer < GC::ZERO)
		{
			checkRoomTimer = GC::CHECK_ROOM_TIMER;
			entity.FindCurrentRoom(rooms);

			if (entity.roomID != playerEntity.roomID)
			{
				differentRoomTimer += GC::CHECK_ROOM_TIMER;
			}
			else
			{
				differentRoomTimer = GC::ZERO;
			}
			
			if (differentRoomTimer >= GC::DIFFERENT_ROOM_DESPAWN_TIMER)
			{
				differentRoomTimer = GC::ZERO;
				entity.isAlive = false;
				active = false;
			}
		}
	}
	else
	{
		active = false;
	}
}

//Check if attack hits player
void Enemy::CheckAttackCollision(GameData& game, Entity& playerEntity)
{
	if (entity.weapon.CheckIfMotionCanDamage() && !playerEntity.invulnerable)
	{
		//Calculate distance to enemy
		Dim2Df position = playerEntity.sprite.getPosition();
		float distanceToEnemy;

		if (entity.weapon.entityIsWeapon)
		{
			distanceToEnemy = CalculateMagnitudeOfVector(entity.sprite.getPosition() - position);
		}
		else
		{
			distanceToEnemy = CalculateMagnitudeOfVector(entity.weapon.sprite.getPosition() - position);
		}

		//If in range, check collision
		if (distanceToEnemy <= GC::CHECK_ATTACK_COLLISION_RANGE)
		{
			bool hitPlayer = false;

			if (entity.weapon.entityIsWeapon)
			{
				hitPlayer = entity.sprite.getGlobalBounds().intersects(playerEntity.sprite.getGlobalBounds());
			}
			else
			{
				hitPlayer = entity.weapon.sprite.getGlobalBounds().intersects(playerEntity.sprite.getGlobalBounds());
			}

			if (hitPlayer)
			{
				//Calculate damage
				unsigned char actualDamage = (unsigned char)round(entity.power * GC::DEFAULT_DAMAGE);
				playerEntity.TakeDamage(actualDamage, entity.facing, GC::ZERO);

				//Immediately activate player invulnerability
				playerEntity.invulnerable = true;
				*game.playerHit = true;

				//Update metrics
				game.metrics.UpdatePlayerDamage(ID, actualDamage);
			}
		}
	}
}