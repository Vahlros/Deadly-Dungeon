#include "weapon.h"
#include "maths.h"

void Projectile::Update(const GameData& game)
{
	//Motion
	if (motion.timer < 0.f)
	{
		active = false;
		motion.active = false;
	}
	else
	{
		//Setup
		motion.UpdateTotals(game);
		sprite.setPosition(origin);

		//Animation
		if (data.type == GC::FIRE_SKULL_PROJECTILE || data.type == GC::FIRE_BALL_PROJECTILE || data.type == GC::FROST_BALL_PROJECTILE)
		{
			anim.UpdateAnimation(sprite, game.elapsed);
		}

		motion.UpdatePosition(&sprite, followingFacing, DirectionalAngle{}, angle, data.radius);
		UpdateRotation(motion, sprite, angle);

		//Time
		if (!motion.loop)
		{
			motion.timer -= game.elapsed;
		}
	}
}

void Projectile::Render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

Dim2Df Projectile::GetFrameMovementVector(const GameData& game)
{
	Dim2Df frameMovementVector = { 0.f, 0.f };

	//Find angle in radians
	DirectionalAngle dirAngle = GetDirectionalAngleFrom360Angle(angle, false);
	ConvertToRadians(dirAngle.angle);

	if (followingFacing)
	{
		//Magnitude is x, direction is y
		frameMovementVector = CalculateVectorOfMagnitude(dirAngle, motion.lineSpeed.x);
		frameMovementVector.x *= motion.lineData->translation.y * game.elapsed;
		frameMovementVector.y *= motion.lineData->translation.y * game.elapsed;
	}
	else
	{
		frameMovementVector = { motion.lineSpeed.x * game.elapsed, motion.lineSpeed.y * game.elapsed };
	}

	return frameMovementVector;
}

void Attack::Init(const GameData& game, sf::Sprite& motionSprite, sf::Sprite* eSprite, DirectionalAngle& entityFacing, float& entityAttackSpeed, float& holdDistance, const bool& eIsWep, Animation* animation)
{
	//Bools
	active = true;
	motionFinished = false;

	//Positioning
	sprite = &motionSprite;
	entityIsWeapon = eIsWep;

	//Setup origin point, where the motion will reset to
	if (movingWithEntity)
	{
		entitySprite = eSprite;
	}
	else
	{
		origin = sprite->getPosition();
	}

	//Motion crucial stats
	facing = &entityFacing;
	attackSpeed = &entityAttackSpeed;
	radius = &holdDistance;
	initialAngle = GetFullAngleInDegrees(*facing);

	if (alternatingSwingDirection)
	{
		if (swingDirection == 1)
		{
			swingDirection = -1;
		}
		else
		{
			swingDirection = 1;
		}
	}

	//Initiate first motion
	motions[0].Init(game, *facing, *attackSpeed, swingDirection, followingFacing);

	//Animation
	anim = animation;
	if (uniqueAnimation && animOnMotion0)
	{
		anim->Init(animData);
	}

	//Angle corrections
	if (arcCentredOnInitialAngle)
	{
		if (followingFacing)
		{
			if (facing->direction == GC::NORTH || facing->direction == GC::EAST) //Right
			{
				arcCentredAngleOffset = -(motions[0].circleData->rotation / 2.f) * swingDirection;
			}
			else //Left
			{
				arcCentredAngleOffset = (motions[0].circleData->rotation / 2.f) * swingDirection;
			}
		}
		else
		{
			if (facing->direction == GC::NORTH || facing->direction == GC::EAST) //Right
			{
				initialAngle -= (motions[0].circleData->rotation / 2.f) * swingDirection;
			}
			else //Left
			{
				initialAngle += (motions[0].circleData->rotation / 2.f) * swingDirection;
			}
		}
	}
}

void Attack::UpdateAttack(const GameData& game, std::vector<Projectile>& projList)
{
	if (motions[0].active) //First motion
	{
		UpdateAttackMotion(game, motions[0]);
	}
	else if (!motions[0].active && hasTwoMotions && attackRelease) //Initiate second motion
	{
		motions[1].Init(game, *facing, *attackSpeed, swingDirection, followingFacing);

		//Animation check for second motion
		if (uniqueAnimation && animOnMotion1)
		{
			anim->Init(animData);
		}

		if (motions[0].circular)
		{
			motions[1].circleTotal = motions[0].circleTotal;
		}
		if (motions[0].line)
		{
			if (followingFacing)
			{
				motions[1].lineTotal.x = motions[0].lineTotal.x * motions[0].lineData->translation.y;
			}
			else
			{
				motions[1].lineTotal.x = motions[0].lineTotal.x;
				motions[1].lineTotal.y = motions[0].lineTotal.y;
			}
		}
		if (motions[0].spin)
		{
			motions[1].spinTotal = motions[0].spinTotal;
		}

		attackRelease = false;
		UpdateAttackMotion(game, motions[1]);
	}
	else if (!motions[0].active && hasTwoMotions && !motions[1].active && !attackRelease && !motionFinished) //Holding attack after first motion
	{
		if (movingWithEntity)
		{
			sprite->setPosition(entitySprite->getPosition());
		}
		else
		{
			sprite->setPosition(origin);
		}

		if (followingFacing)
		{
			initialAngle = GetFullAngleInDegrees(*facing);
		}

		motions[0].UpdatePosition(sprite, followingFacing, *facing, initialAngle, *radius);

		if (entityIsWeapon)
		{
			UpdateRotation(motions[0], *sprite, GC::ZERO);
		}
		else
		{
			UpdateRotation(motions[0], *sprite, initialAngle);
		}
	}
	else if (motions[1].active && hasTwoMotions) //Second motion
	{
		UpdateAttackMotion(game, motions[1]);

		if (!motions[1].active)
		{
			motionFinished = true;
		}
	}
	else if (!hasTwoMotions || motionFinished) //Finish motion
	{
		if (summonProjectile)
		{
			if (!followingFacing)
			{
				initialAngle = GetFullAngleInDegrees(*facing);
			}

			SpawnProjectiles(game, projList);
		}

		Stop();
	}
}

void Attack::UpdateAttackMotion(const GameData& game, Motion& motion)
{
	if (motion.timer < 0.f)
	{
		motion.active = false;
	}
	else
	{
		//Setup
		motion.UpdateTotals(game);

		if (followingFacing)
		{
			initialAngle = GetFullAngleInDegrees(*facing);

			if (arcCentredOnInitialAngle)
			{
				initialAngle += arcCentredAngleOffset;
			}
		}

		//Motion position
		if (movingWithEntity)
		{
			sprite->setPosition(entitySprite->getPosition());
		}
		else
		{
			sprite->setPosition(origin);
		}

		motion.UpdatePosition(sprite, followingFacing, *facing, initialAngle, *radius);

		if (entityIsWeapon)
		{
			UpdateRotation(motion, *sprite, GC::ZERO);
		}
		else
		{
			UpdateRotation(motion, *sprite, initialAngle);
		}

		if (followingFacing && arcCentredOnInitialAngle)
		{
			initialAngle -= arcCentredAngleOffset;
		}

		//Time
		if (!motion.loop)
		{
			//motion.timer -= game.elapsed;
			motion.timer -= GC::APPROX_ELAPSED;
		}
	}
}

void Attack::SpawnProjectiles(const GameData& game, std::vector<Projectile>& projList)
{
	//Setup
	unsigned char projectileCount = projectileData->num;
	short index = 0;
	bool hasSpread;
	std::vector<float> spreadList(projectileCount);

	//Calculate spread
	if (projectileCount > 1)
	{
		hasSpread = true;
		bool unevenProjectileNum = projectileCount % 2;
		unsigned char spreadCounter = projectileCount;

		while (spreadCounter > 0)
		{
			float spread = projectileData->spread;

			if (unevenProjectileNum)
			{
				//Evenly spread around the initialAngle except from the last projectile
				if (spreadCounter != 1)
				{
					spread *= spreadCounter / 2;

					if ((spreadCounter % 2) == 1)
					{
						spread *= -1;
					}
				}
				else
				{
					spread = 0.f;
				}
			}
			else
			{
				//Evenly spread around the initialAngle
				spread *= ((spreadCounter + 1) / 2);
				spread -= projectileData->spread / 2.f;

				if ((spreadCounter % 2) == 1)
				{
					spread *= -1;
				}
			}

			spreadList[spreadCounter - 1] = spread;
			spreadCounter -= 1;
		}
	}
	else
	{
		hasSpread = false;
	}

	//Find an available projectile and spawn it
	while (projectileCount > 0)
	{
		if (projList[index].active)
		{
			index += 1;
		}
		else if (index < GC::MAX_PROJECTILES)
		{
			projList[index].active = true;
			projList[index].data = *projectileData;
			projList[index].motion = *projList[index].data.motion;

			if (projectileData->type == GC::WEAPON_PROJECTILE)
			{
				projList[index].sprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);
				projList[index].sprite.setTextureRect(sprite->getTextureRect());
				projList[index].sprite.setOrigin(sprite->getOrigin());
				projList[index].sprite.setPosition(sprite->getPosition());
				projList[index].origin = sprite->getPosition();
			}
			else if (projectileData->type == GC::FIRE_SKULL_PROJECTILE)
			{
				projList[index].sprite.setTexture(game.textures[GC::FIRE_SKULL_TEXTURE]);
				projList[index].sprite.setTextureRect(GC::FIRE_SKULL_BODY_RECT);
				projList[index].sprite.setOrigin(GC::FIRE_SKULL_BODY_CENTRE);
				projList[index].sprite.setPosition(sprite->getPosition());
				projList[index].origin = sprite->getPosition();

				projList[index].anim.data = &GC::FIRE_SKULL_ANIM;
				projList[index].anim.currentFrame = projList[index].anim.data->startFrame;
			}
			else if (projectileData->type == GC::FIRE_BALL_PROJECTILE)
			{
				projList[index].sprite.setTexture(game.textures[GC::FIRE_BALL_TEXTURE]);
				projList[index].sprite.setTextureRect(GC::FIRE_BALL_BODY_RECT);
				projList[index].sprite.setOrigin(GC::FIRE_BALL_BODY_CENTRE);
				projList[index].sprite.setPosition(sprite->getPosition());
				projList[index].origin = sprite->getPosition();

				projList[index].anim.data = &GC::FIRE_BALL_ANIM;
				projList[index].anim.currentFrame = projList[index].anim.data->startFrame;
			}
			else //if (projectileData->type == GC::FROST_BALL_PROJECTILE)
			{
				projList[index].sprite.setTexture(game.textures[GC::FROST_BALL_TEXTURE]);
				projList[index].sprite.setTextureRect(GC::FROST_BALL_BODY_RECT);
				projList[index].sprite.setOrigin(GC::FROST_BALL_BODY_CENTRE);
				projList[index].sprite.setPosition(sprite->getPosition());
				projList[index].origin = sprite->getPosition();

				projList[index].anim.data = &GC::FIRE_BALL_ANIM;
				projList[index].anim.currentFrame = projList[index].anim.data->startFrame;
			}

			projList[index].playerProjectile = projectileShotByPlayer;
			if (!projectileShotByPlayer)
			{
				projList[index].enemyID = enemyID;
			}

			if (hasSpread)
			{
				projList[index].angle = initialAngle + spreadList[projectileCount - 1];
			}
			else
			{
				projList[index].angle = initialAngle;
			}
			ConstrainAngle(projList[index].angle, false);

			projList[index].motion.Init(game, GetDirectionalAngleFrom360Angle(projList[index].angle, false), *attackSpeed, 1, true);
			projList[index].sprite.setRotation(projList[index].angle);

			//For Motion calculations, projectiles do not actively follow entity's facing DirectionalAngle.
			//Instead this ensures that the projectile follows the initialAngle of the attack
			projList[index].followingFacing = true;

			projectileCount -= 1;
			index += 1;
		}
		else
		{
			break;
		}
	}
}

void Attack::Stop()
{
	active = false;
	motions[0].ResetTotals();
	motions[1].ResetTotals();

	if (uniqueAnimation) //Only enemies so default to enemy animations
	{
		anim->Init(&GC::ENEMY_ANIM_IDLE);
	}
}

void Weapon::Init(const GameData& game, const bool& isPlayer, Animation& entityAnim)
{
	active = true;
	attack0.projectileShotByPlayer = isPlayer;
	attack0.anim = &entityAnim;
	attack1.projectileShotByPlayer = isPlayer;
	attack1.anim = &entityAnim;

	if (entityIsWeapon)
	{
		visible = false;
		attack0Range = attack0.range;
		attack1Range = attack1.range;
	}
	else
	{
		//Weapon setup
		visible = true;

		//SFML
		sprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);
		sprite.setTextureRect(*textureRect);
		sprite.setOrigin(*origin);

		//Range calculation
		if (attack0.range == -1)
		{
			attack0Range = -1;
		}
		else
		{
			attack0Range = (short)floor(origin->y + (GC::WEAPON_HOVER * GC::TILE_SIZE) + attack0.range);
		}

		if (attack1.range == -1)
		{
			attack1Range = -1;
		}
		else
		{
			attack1Range = (short)floor(origin->y + (GC::WEAPON_HOVER * GC::TILE_SIZE) + attack1.range);
		}
	}
}

void Weapon::UpdateHoldPosition(const DirectionalAngle& facing, const Dim2Df holdOrigin)
{
	sprite.setPosition(holdOrigin + CalculateCircularMotionVector(holdDistance, GetFullAngleInRads(facing)));
}

void Weapon::UpdateHoldRotation(const DirectionalAngle& facing)
{
	float rotation = GetFullAngleInDegrees(facing);

	if (rotation < GC::DEGREES_180)
	{
		sprite.setRotation(GC::WEAPON_HOVER_ROTATION);
	}
	else
	{
		sprite.setRotation(-GC::WEAPON_HOVER_ROTATION);
	}
}

bool Weapon::CheckIfMotionCanDamage()
{
	//Check if the motion can damage
	bool canDamage = false;

	if (attack0.active)
	{
		if (attack0.motions[0].active && attack0.motions[0].damage)
		{
			canDamage = true;
		}
		else if (attack0.hasTwoMotions && attack0.motions[1].active && attack0.motions[1].damage)
		{
			canDamage = true;
		}
	}
	else if (attack1.active)
	{
		if (attack1.motions[0].active && attack1.motions[0].damage)
		{
			canDamage = true;
		}
		else if (attack1.hasTwoMotions && attack1.motions[1].active && attack1.motions[1].damage)
		{
			canDamage = true;
		}
	}

	return canDamage;
}

void Weapon::StopAttackIfTrue(const bool& stop)
{
	if (stop)
	{
		attack0.Stop();
		attack1.Stop();
	}
}

void UpdateRotation(const Motion& motion, sf::Sprite& sprite, const float& initialAngle)
{
	if (motion.spin)
	{
		sprite.setRotation(initialAngle + motion.spinTotal);
	}
	else //if (followingFacing)
	{
		sprite.setRotation(initialAngle);
	}
}