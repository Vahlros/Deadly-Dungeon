#include "entity.h"
#include "maths.h"

//Initiates an attack
void Entity::InitAttack(const GameData& game, const char& attack)
{
	canAttack = false;
	weapon.attacking = true;

	if (attack == GC::FIRST_ATTACK)
	{
		if (weapon.entityIsWeapon)
		{
			weapon.attack0.Init(game, sprite, nullptr, facing, attackSpeed, weapon.holdDistance, true, &anim);
		}
		else
		{
			weapon.attack0.Init(game, weapon.sprite, &sprite, facing, attackSpeed, weapon.holdDistance, false, &anim);
		}
	}
	else if (weapon.hasTwoAttacks && attack == GC::SECOND_ATTACK)
	{
		if (weapon.entityIsWeapon)
		{
			weapon.attack1.Init(game, sprite, nullptr, facing, attackSpeed, weapon.holdDistance, true, &anim);
		}
		else
		{
			weapon.attack1.Init(game, weapon.sprite, &sprite, facing, attackSpeed, weapon.holdDistance, false, &anim);
		}
	}
}

//Initiates a knockback
void Entity::InitKnockback(const DirectionalAngle& facing, const float& knockPower)
{
	knockback = true;
	knock.movementVector = CalculateVectorOfMagnitude(facing, knockPower);
	knock.timer = GC::KNOCKBACK_TIMER;
	invulnerable = true;
	invulnerabilityTimer = GC::KNOCKBACK_INVULNERABILITY;
}

//Updates knockback
void Entity::UpdateKnockback(const GameData& game)
{
	knock.timer -= game.elapsed;
	invulnerabilityTimer -= game.elapsed;

	if (knock.timer < 0.f)
	{
		knockback = false;
	}

	if (invulnerabilityTimer < 0.f)
	{
		invulnerable = false;
	}
}

//Move the entity, also updates knockback
void Entity::Move(const GameData& game)
{
	if (knockback)
	{
		frameMovementVector.x = (int)roundf(knock.movementVector.x * game.elapsed);
		frameMovementVector.y = (int)roundf(knock.movementVector.y * game.elapsed);

		UpdateKnockback(game);
	}
	else
	{
		frameMovementVector.x = (int)roundf(movementVector.x * game.elapsed);
		frameMovementVector.y = (int)roundf(movementVector.y * game.elapsed);
	}
	
	CheckMapCollision(game, false);

	sprite.move(Dim2Df(frameMovementVector));
}

//Checks if movement is valid, using rectangle intersections
void Entity::CheckMapCollision(const GameData& game, const bool& entityBodyAttack)
{
	bool movingLeft = false, movingRight = false, movingUp = false, movingDown = false;

	if (entityBodyAttack) //Attack movement
	{
		//Get angle
		float angle;

		if (weapon.attack0.active)
		{
			angle = weapon.attack0.initialAngle;
		}
		else //if (weapon.attack1.active)
		{
			angle = weapon.attack1.initialAngle;
		}
		ConstrainAngle(angle, false);

		//Get movement booleans
		if ((angle >= GC::DEGREES_225) && (angle <= GC::DEGREES_315))
		{
			movingLeft = true;
		}
		else if ((angle >= GC::DEGREES_45) && (angle <= GC::DEGREES_135))
		{
			movingRight = true;
		}

		if ((angle >= GC::DEGREES_315) || (angle <= GC::DEGREES_45))
		{
			movingUp = true;
		}
		else if ((angle >= GC::DEGREES_135) && (angle <= GC::DEGREES_225))
		{
			movingDown = true;
		}
	}
	else //Normal movement
	{
		//Get movement booleans
		if (frameMovementVector.x < 0)
		{
			movingLeft = true;
		}
		else if (frameMovementVector.x > 0)
		{
			movingRight = true;
		}

		if (frameMovementVector.y < 0)
		{
			movingUp = true;
		}
		else if (frameMovementVector.y > 0)
		{
			movingDown = true;
		}
	}

	//Initialize collision rectangles, left and right rects based on the direction the entity is moving
	//example: When facing the south/down direction, left is south east and right is south west
	sf::IntRect collisionBox = sf::IntRect(sprite.getGlobalBounds()); //Entity's collision rectangle
	collisionBox.top = collisionBox.top + collisionBox.height - GC::FEET_COLLISION_HEIGHT;
	collisionBox.height = GC::FEET_COLLISION_HEIGHT;

	//Enlarge collision box for attacks 
	if (entityBodyAttack)
	{
		collisionBox.left -= GC::ENEMY_ATTACK_C_OFFSET;
		collisionBox.width += GC::ENEMY_ATTACK_C_OFFSET * 2;
		collisionBox.top -= GC::ENEMY_ATTACK_C_OFFSET;
		collisionBox.height += GC::ENEMY_ATTACK_C_OFFSET * 2;
	}
	

	sf::IntRect leftTileRect = { 0, 0, 0, 0 }; //Map collision rectangle, to the left of the direction of movement
	sf::IntRect rightTileRect = { 0, 0, 0, 0 }; //Map collision rectangle, to the right of the direction of movement
	sf::IntRect intersection = { 0, 0, 0, 0 }; //Intersection rectangle
	Dim2Di leftPoint = { 0, 0 }; //Left point coordinates
	Dim2Di rightPoint = { 0, 0 }; //Right point coordinates
	unsigned char leftTile = 0; //Left tile ID
	unsigned char rightTile = 0; //Right tile ID
	bool collided = false; //If the entity has collided with the map

	//Find collision rectangles and offset them slightly for smooth wall sliding
	//Increase collision box in exact direction and decrease in perpendicular direction
	if (movingUp)
	{
		leftPoint = { (collisionBox.left + GC::C_OFFSET) / GC::TILE_SIZE, (collisionBox.top + frameMovementVector.y) / GC::TILE_SIZE }; //Top left
		leftTile = game.collisionMap[leftPoint.y][leftPoint.x];

		if (leftTile != GC::C_FREE_MOVEMENT)
		{
			if (leftTile == GC::C_WALL)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
			}
			else if (leftTile == GC::C_WALL_SIDE_LEFT)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH - GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (leftTile == GC::C_FOUNTAIN_BASIN)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT + GC::C_OFFSET };
			}
			else if (leftTile == GC::C_COLUMN_BASE)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::COLUMN_BASE_HEIGHT + GC::C_OFFSET };
			}

			if (collisionBox.intersects(leftTileRect, intersection))
			{
				collided = true;
				frameMovementVector.y = 0;
			}
		}

		if (!collided)
		{
			rightPoint = { (collisionBox.left + collisionBox.width - GC::C_OFFSET) / GC::TILE_SIZE, (collisionBox.top + frameMovementVector.y) / GC::TILE_SIZE }; //Top right
			rightTile = game.collisionMap[rightPoint.y][rightPoint.x];

			if (rightTile != GC::C_FREE_MOVEMENT)
			{
				if (rightTile == GC::C_WALL)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
				}
				else if (rightTile == GC::C_WALL_SIDE_RIGHT)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH - GC::C_OFFSET), rightPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH - GC::C_OFFSET, GC::TILE_SIZE };
				}
				else if (rightTile == GC::C_FOUNTAIN_BASIN)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT + GC::C_OFFSET };
				}
				else if (rightTile == GC::C_COLUMN_BASE)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::COLUMN_BASE_HEIGHT + GC::C_OFFSET };
				}

				if (collisionBox.intersects(rightTileRect, intersection))
				{
					collided = true;
					frameMovementVector.y = 0;
				}
			}
		}
	}
	else if (movingDown)
	{
		leftPoint = { (collisionBox.left + collisionBox.width - GC::C_OFFSET) / GC::TILE_SIZE, (collisionBox.top + collisionBox.height + frameMovementVector.y) / GC::TILE_SIZE }; //Bottom right
		leftTile = game.collisionMap[leftPoint.y][leftPoint.x];

		if (leftTile != GC::C_FREE_MOVEMENT)
		{
			if (leftTile == GC::C_WALL)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (leftPoint.y * GC::TILE_SIZE) - GC::C_OFFSET, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
			}
			else if ((leftTile == GC::C_WALL_TOP) || (leftTile == GC::C_CORNER_BOTTOM_RIGHT) || (leftTile == GC::C_CORNER_BOTTOM_LEFT))
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::WALL_TOP_HEIGHT + GC::C_OFFSET };
			}
			else if (leftTile == GC::C_WALL_TOP_BOTTOM_RIGHT)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH - GC::C_OFFSET), (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET),
					GC::WALL_SIDE_WIDTH - GC::C_OFFSET, GC::WALL_TOP_HEIGHT + GC::C_OFFSET };
			}
			else if (leftTile == GC::C_FOUNTAIN_TOP)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET };
			}
			else if (leftTile == GC::C_COLUMN_TOP)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET };
			}

			if (collisionBox.intersects(leftTileRect, intersection))
			{
				collided = true;
				frameMovementVector.y = 0;
			}
		}

		if (!collided)
		{
			rightPoint = { (collisionBox.left + GC::C_OFFSET) / GC::TILE_SIZE, (collisionBox.top + collisionBox.height + frameMovementVector.y) / GC::TILE_SIZE }; //Bottom left
			rightTile = game.collisionMap[rightPoint.y][rightPoint.x];

			if (rightTile != GC::C_FREE_MOVEMENT)
			{
				if (rightTile == GC::C_WALL)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, (rightPoint.y * GC::TILE_SIZE) - GC::C_OFFSET, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
				}
				else if ((rightTile == GC::C_WALL_TOP) || (rightTile == GC::C_CORNER_BOTTOM_LEFT) || (rightTile == GC::C_CORNER_BOTTOM_RIGHT))
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::WALL_TOP_HEIGHT + GC::C_OFFSET };
				}
				else if (rightTile == GC::C_WALL_TOP_BOTTOM_LEFT)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET),
						GC::WALL_SIDE_WIDTH - GC::C_OFFSET, (GC::WALL_TOP_HEIGHT + GC::C_OFFSET) };
				}
				else if (rightTile == GC::C_FOUNTAIN_TOP)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, (GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET) };
				}
				else if (rightTile == GC::C_COLUMN_TOP)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, (GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET) };
				}

				if (collisionBox.intersects(rightTileRect, intersection))
				{
					collided = true;
					frameMovementVector.y = 0;
				}
			}
		}
	}

	//Check attack collision and reset
	if (entityBodyAttack)
	{
		StopAttackIfMapCollided(collided);
	}
	collided = false;

	//Reset collision rects
	leftTileRect = { 0, 0, 0, 0 };
	rightTileRect = { 0, 0, 0, 0 };

	if (movingLeft)
	{
		leftPoint = { (collisionBox.left + frameMovementVector.x) / GC::TILE_SIZE, (collisionBox.top + collisionBox.height - GC::C_OFFSET) / GC::TILE_SIZE }; //Bottom left
		leftTile = game.collisionMap[leftPoint.y][leftPoint.x];

		if (leftTile != GC::C_FREE_MOVEMENT)
		{
			if (leftTile == GC::C_WALL)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (leftTile == GC::C_WALL_TOP)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT - GC::C_OFFSET), GC::TILE_SIZE, GC::WALL_TOP_HEIGHT - GC::C_OFFSET };
			}
			else if ((leftTile == GC::C_WALL_SIDE_LEFT) || (leftTile == GC::C_CORNER_BOTTOM_LEFT))
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if ((leftTile == GC::C_WALL_SIDE_RIGHT) || (leftTile == GC::C_CORNER_BOTTOM_RIGHT))
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH + GC::C_OFFSET), leftPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (leftTile == GC::C_WALL_TOP_BOTTOM_LEFT)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::WALL_TOP_HEIGHT,
					(GC::WALL_SIDE_WIDTH + GC::C_OFFSET), GC::WALL_TOP_HEIGHT };
			}
			else if (leftTile == GC::C_FOUNTAIN_TOP)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::FOUNTAIN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_TOP_HEIGHT };
			}
			else if (leftTile == GC::C_COLUMN_TOP)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, (leftPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::COLUMN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::COLUMN_TOP_HEIGHT };
			}

			if (collisionBox.intersects(leftTileRect, intersection))
			{
				collided = true;
				frameMovementVector.x = 0;
			}
		}

		if (!collided)
		{
			rightPoint = { (collisionBox.left + frameMovementVector.x) / GC::TILE_SIZE, (collisionBox.top + GC::C_OFFSET) / GC::TILE_SIZE }; //Top left
			rightTile = game.collisionMap[rightPoint.y][rightPoint.x];

			if (rightTile != GC::C_FREE_MOVEMENT)
			{
				if (rightTile == GC::C_WALL)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE - GC::C_OFFSET };
				}
				else if (rightTile == GC::C_WALL_SIDE_LEFT)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, rightPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET, GC::TILE_SIZE };
				}
				else if (rightTile == GC::C_WALL_SIDE_RIGHT)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH + GC::C_OFFSET), rightPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
				}
				else if (rightTile == GC::C_FOUNTAIN_BASIN)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT - GC::C_OFFSET };
				}
				else if (rightTile == GC::C_COLUMN_BASE)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::COLUMN_BASE_HEIGHT - GC::C_OFFSET };
				}

				if (collisionBox.intersects(rightTileRect, intersection))
				{
					collided = true;
					frameMovementVector.x = 0;
				}
			}
		}
	}
	else if (movingRight)
	{
		leftPoint = { (collisionBox.left + collisionBox.width + frameMovementVector.x) / GC::TILE_SIZE, (collisionBox.top + GC::C_OFFSET) / GC::TILE_SIZE }; //Top right
		leftTile = game.collisionMap[leftPoint.y][leftPoint.x];

		if (leftTile != GC::C_FREE_MOVEMENT)
		{
			if (leftTile == GC::C_WALL)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (leftTile == GC::C_WALL_SIDE_LEFT)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, leftPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (leftTile == GC::C_WALL_SIDE_RIGHT)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH + GC::C_OFFSET), leftPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (leftTile == GC::C_FOUNTAIN_BASIN)
			{
				leftTileRect = { (leftPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT - GC::C_OFFSET };
			}
			else if (leftTile == GC::C_COLUMN_BASE)
			{
				leftTileRect = { leftPoint.x * GC::TILE_SIZE, leftPoint.y * GC::TILE_SIZE, GC::TILE_SIZE, GC::COLUMN_BASE_HEIGHT - GC::C_OFFSET };
			}

			if (collisionBox.intersects(leftTileRect, intersection))
			{
				collided = true;
				frameMovementVector.x = 0;
			}
		}

		if (!collided)
		{
			rightPoint = { (collisionBox.left + collisionBox.width + frameMovementVector.x) / GC::TILE_SIZE, (collisionBox.top + collisionBox.height - GC::C_OFFSET) / GC::TILE_SIZE }; //Bottom right
			rightTile = game.collisionMap[rightPoint.y][rightPoint.x];

			if (rightTile != GC::C_FREE_MOVEMENT)
			{
				if (rightTile == GC::C_WALL)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, rightPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE };
				}
				else if (rightTile == GC::C_WALL_TOP)
				{
					rightTileRect = { rightPoint.x * GC::TILE_SIZE, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT - GC::C_OFFSET), GC::TILE_SIZE, GC::WALL_TOP_HEIGHT - GC::C_OFFSET };
				}
				else if ((rightTile == GC::C_WALL_SIDE_LEFT) || (rightTile == GC::C_CORNER_BOTTOM_LEFT))
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, rightPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
				}
				else if ((rightTile == GC::C_WALL_SIDE_RIGHT) || (rightTile == GC::C_CORNER_BOTTOM_RIGHT))
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH + GC::C_OFFSET), rightPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH + GC::C_OFFSET, GC::TILE_SIZE };
				}
				else if (rightTile == GC::C_WALL_TOP_BOTTOM_RIGHT)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH + GC::C_OFFSET), (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::WALL_TOP_HEIGHT,
						(GC::WALL_SIDE_WIDTH + GC::C_OFFSET), GC::WALL_TOP_HEIGHT };
				}
				else if (rightTile == GC::C_FOUNTAIN_TOP)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::FOUNTAIN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_TOP_HEIGHT };
				}
				else if (rightTile == GC::C_COLUMN_TOP)
				{
					rightTileRect = { (rightPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, (rightPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::COLUMN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::COLUMN_TOP_HEIGHT };
				}

				if (collisionBox.intersects(rightTileRect, intersection))
				{
					collided = true;
					frameMovementVector.x = 0;
				}
			}
		}
	}

	//Check attack collision
	if (entityBodyAttack)
	{
		StopAttackIfMapCollided(collided);
	}
}

//Render the entity if it's on the rendered map area
void Entity::Render(sf::RenderWindow& window, const GameData& game)
{
	anim.UpdateAnimation(sprite, game.elapsed);

	//Flip and move sprite based on facing
	if ((facing.direction == GC::NORTH) || (facing.direction == GC::EAST))
	{
		if (!facingRight)
		{
			sprite.setScale(1.f, 1.f);
			facingRight = true;
		}
	}
	else
	{
		if (facingRight)
		{
			sprite.setScale(-1.f, 1.f);
			facingRight = false;
		}
	}

	if (weapon.visible)
	{
		if ((facing.direction == GC::WEST) || (facing.direction == GC::NORTH)) //Weapon is behind the player
		{
			window.draw(weapon.sprite);
			window.draw(sprite);
		}
		else //Weapon is infront of the player
		{
			window.draw(sprite);
			window.draw(weapon.sprite);
		}
	}
	else
	{
		window.draw(sprite);
	}
}

//Updates any ongoing attacks
void Entity::UpdateAttacks(const GameData& game, std::vector<Projectile>& proj)
{
	if (weapon.attack0.active)
	{
		weapon.attack0.UpdateAttack(game, proj);
	}
	else if (weapon.hasTwoAttacks && weapon.attack1.active)
	{
		weapon.attack1.UpdateAttack(game, proj);
	}
	else
	{
		weapon.attacking = false;
		canAttack = true;

		if (weapon.entityIsWeapon)
		{
			sprite.setRotation(GC::ZERO);
		}
	}
}

//Update the weapon's state
void Entity::UpdateWeapon(const GameData& game, std::vector<Projectile>& proj)
{
	float x = 0.f, y = 0.f;

	if (weapon.attacking)
	{
		UpdateAttacks(game, proj);

		//Weapon movement correction
		if (moving && weapon.visible)
		{
			x -= frameMovementVector.x;
			y -= frameMovementVector.y;
		}
		
		//Check map collision for entity attacks
		if (weapon.attacking && weapon.entityIsWeapon)
		{
			CheckMapCollision(game, true);
		}
	}
	else
	{
		weapon.UpdateHoldPosition(facing, sprite.getPosition());
		weapon.UpdateHoldRotation(facing);
	}
	
	//Weapon bobbing along to sprite's animation, this needs replacing with a more entity-agnostic method
	if (weapon.visible)
	{
		if (anim.currentFrame == 5)
		{
			y -= 1;
		}
		else if (anim.currentFrame == 7)
		{
			y += 1;
		}
		else if (anim.currentFrame < 4 && anim.currentFrame != 0)
		{
			y -= (anim.currentFrame % 2) - 2;
		}
	}

	weapon.sprite.move({ x, y });
}

//Entity takes damage
bool Entity::TakeDamage(const unsigned char& damage, const DirectionalAngle& facing, const float& knockPower)
{
	health -= damage;

	if (health <= 0)
	{
		isAlive = false;
	}
	else if (!isPlayer && !knock.immovable)
	{
		InitKnockback(facing, knockPower);
	}

	return !isAlive;
}

//Stop entity attack if map collision
void Entity::StopAttackIfMapCollided(const bool& collided)
{
	if (collided)
	{
		weapon.attack0.Stop();
		weapon.attack1.Stop();
	}
}