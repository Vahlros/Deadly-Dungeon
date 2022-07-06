#include "collision.h"
#include "maths.h"

//Finds the direction of movement
void GetMovementBools(const int& type, const Weapon& weapon, const Dim2Df& frameMovementVector, const float& projectileAngle, bool& movingLeft, bool& movingRight, bool& movingUp, bool& movingDown)
{
	if (type == GC::C_ENTITY_BODY_ATTACK)
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
	else //Normal entity movement
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
}

bool CheckCollisionLeft(const GameData& game, const sf::FloatRect& collisionBox, Dim2Df& frameMovementVector, const float& wallSideWidth)
{
	sf::FloatRect tileRect = { 0, 0, 0, 0 }; //Map collision rectangle
	sf::FloatRect intersection = { 0, 0, 0, 0 }; //Intersection rectangle
	Dim2Di collisionPointInt = {}; //Which tile the sprite will be on after movement
	Dim2Df collisionPoint = { 0.f, 0.f }; //Which tile the sprite will be on after movement
	int tile = 0; //Collision ID of the tile at the collisionPoint
	bool collided = false; //If the entity has collided with the map


	collisionPointInt = { (int)floor(collisionBox.left + frameMovementVector.x) / GC::TILE_SIZE, (int)floor(collisionBox.top + collisionBox.height - GC::C_OFFSET) / GC::TILE_SIZE }; //Bottom left
	collisionPoint = Dim2Df(collisionPointInt);
	tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

	if (tile != GC::C_FREE_MOVEMENT)
	{
		if (tile == GC::C_WALL)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if (tile == GC::C_WALL_TOP)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT - GC::C_OFFSET), GC::TILE_SIZE, GC::WALL_TOP_HEIGHT - GC::C_OFFSET };
		}
		else if ((tile == GC::C_WALL_SIDE_LEFT) || (tile == GC::C_CORNER_BOTTOM_LEFT))
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if ((tile == GC::C_WALL_SIDE_RIGHT) || (tile == GC::C_CORNER_BOTTOM_RIGHT))
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (wallSideWidth + GC::C_OFFSET), collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if (tile == GC::C_WALL_TOP_BOTTOM_LEFT)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::WALL_TOP_HEIGHT,
				(wallSideWidth + GC::C_OFFSET), GC::WALL_TOP_HEIGHT };
		}
		else if (tile == GC::C_FOUNTAIN_TOP)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::FOUNTAIN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_TOP_HEIGHT };
		}
		else if (tile == GC::C_COLUMN_TOP)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::COLUMN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::COLUMN_TOP_HEIGHT };
		}

		if (collisionBox.intersects(tileRect, intersection))
		{
			collided = true;
			frameMovementVector.x = 0;
		}
	}

	if (!collided)
	{
		collisionPointInt = { (int)floor(collisionBox.left + frameMovementVector.x) / GC::TILE_SIZE, (int)floor(collisionBox.top + GC::C_OFFSET) / GC::TILE_SIZE }; //Top left
		collisionPoint = Dim2Df(collisionPointInt);
		tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

		if (tile != GC::C_FREE_MOVEMENT)
		{
			if (tile == GC::C_WALL)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE - GC::C_OFFSET };
			}
			else if (tile == GC::C_WALL_SIDE_LEFT)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (tile == GC::C_WALL_SIDE_RIGHT)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (wallSideWidth + GC::C_OFFSET), collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (tile == GC::C_FOUNTAIN_BASIN)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT - GC::C_OFFSET };
			}
			else if (tile == GC::C_COLUMN_BASE)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::COLUMN_BASE_HEIGHT - GC::C_OFFSET };
			}

			if (collisionBox.intersects(tileRect, intersection))
			{
				collided = true;
				frameMovementVector.x = 0;
			}
		}
	}

	return collided;
}

bool CheckCollisionRight(const GameData& game, const sf::FloatRect& collisionBox, Dim2Df& frameMovementVector, const float& wallSideWidth)
{
	sf::FloatRect tileRect = { 0, 0, 0, 0 }; //Map collision rectangle
	sf::FloatRect intersection = { 0, 0, 0, 0 }; //Intersection rectangle
	Dim2Di collisionPointInt = {}; //Which tile the sprite will be on after movement
	Dim2Df collisionPoint = { 0.f, 0.f }; //Which tile the sprite will be on after movement
	int tile = 0; //Collision ID of the tile at the collisionPoint
	bool collided = false; //If the entity has collided with the map

	collisionPointInt = { (int)floor(collisionBox.left + collisionBox.width + frameMovementVector.x) / GC::TILE_SIZE, (int)floor(collisionBox.top + GC::C_OFFSET) / GC::TILE_SIZE }; //Top right
	collisionPoint = Dim2Df(collisionPointInt);
	tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

	if (tile != GC::C_FREE_MOVEMENT)
	{
		if (tile == GC::C_WALL)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if (tile == GC::C_WALL_SIDE_LEFT)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if (tile == GC::C_WALL_SIDE_RIGHT)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (wallSideWidth + GC::C_OFFSET), collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if (tile == GC::C_FOUNTAIN_BASIN)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT - GC::C_OFFSET };
		}
		else if (tile == GC::C_COLUMN_BASE)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE, GC::COLUMN_BASE_HEIGHT - GC::C_OFFSET };
		}

		if (collisionBox.intersects(tileRect, intersection))
		{
			collided = true;
			frameMovementVector.x = 0;
		}
	}

	if (!collided)
	{
		collisionPointInt = { (int)floor(collisionBox.left + collisionBox.width + frameMovementVector.x) / GC::TILE_SIZE, (int)floor(collisionBox.top + collisionBox.height - GC::C_OFFSET) / GC::TILE_SIZE }; //Bottom right
		collisionPoint = Dim2Df(collisionPointInt);
		tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

		if (tile != GC::C_FREE_MOVEMENT)
		{
			if (tile == GC::C_WALL)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (tile == GC::C_WALL_TOP)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT - GC::C_OFFSET), GC::TILE_SIZE, GC::WALL_TOP_HEIGHT - GC::C_OFFSET };
			}
			else if ((tile == GC::C_WALL_SIDE_LEFT) || (tile == GC::C_CORNER_BOTTOM_LEFT))
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if ((tile == GC::C_WALL_SIDE_RIGHT) || (tile == GC::C_CORNER_BOTTOM_RIGHT))
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (wallSideWidth + GC::C_OFFSET), collisionPoint.y * GC::TILE_SIZE, wallSideWidth + GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (tile == GC::C_WALL_TOP_BOTTOM_RIGHT)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (wallSideWidth + GC::C_OFFSET), (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::WALL_TOP_HEIGHT,
					(wallSideWidth + GC::C_OFFSET), GC::WALL_TOP_HEIGHT };
			}
			else if (tile == GC::C_FOUNTAIN_TOP)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::FOUNTAIN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::FOUNTAIN_TOP_HEIGHT };
			}
			else if (tile == GC::C_COLUMN_TOP)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) - GC::C_OFFSET, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - GC::COLUMN_TOP_HEIGHT, GC::TILE_SIZE + GC::C_OFFSET, GC::COLUMN_TOP_HEIGHT };
			}

			if (collisionBox.intersects(tileRect, intersection))
			{
				collided = true;
				frameMovementVector.x = 0;
			}
		}
	}

	return collided;
}

bool CheckCollisionUp(const GameData& game, const sf::FloatRect& collisionBox, Dim2Df& frameMovementVector, const float& wallSideWidth)
{
	sf::FloatRect tileRect = { 0, 0, 0, 0 }; //Map collision rectangle
	sf::FloatRect intersection = { 0, 0, 0, 0 }; //Intersection rectangle
	Dim2Di collisionPointInt = {}; //Which tile the sprite will be on after movement
	Dim2Df collisionPoint = { 0.f, 0.f }; //Which tile the sprite will be on after movement
	int tile = 0; //Collision ID of the tile at the collisionPoint
	bool collided = false; //If the entity has collided with the map

	collisionPointInt = { (int)floor(collisionBox.left + GC::C_OFFSET) / GC::TILE_SIZE, (int)floor(collisionBox.top + frameMovementVector.y) / GC::TILE_SIZE }; //Top left
	collisionPoint = Dim2Df(collisionPointInt);
	tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

	if (tile != GC::C_FREE_MOVEMENT)
	{
		if (tile == GC::C_WALL)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
		}
		else if (tile == GC::C_WALL_SIDE_LEFT)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH - GC::C_OFFSET, GC::TILE_SIZE };
		}
		else if (tile == GC::C_FOUNTAIN_BASIN)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT + GC::C_OFFSET };
		}
		else if (tile == GC::C_COLUMN_BASE)
		{
			tileRect = { collisionPoint.x * GC::TILE_SIZE, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::COLUMN_BASE_HEIGHT + GC::C_OFFSET };
		}

		if (collisionBox.intersects(tileRect, intersection))
		{
			collided = true;
			frameMovementVector.y = 0;
		}
	}

	if (!collided)
	{
		collisionPointInt = { (int)floor(collisionBox.left + collisionBox.width - GC::C_OFFSET) / GC::TILE_SIZE, (int)floor(collisionBox.top + frameMovementVector.y) / GC::TILE_SIZE }; //Top right
		collisionPoint = Dim2Df(collisionPointInt);
		tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

		if (tile != GC::C_FREE_MOVEMENT)
		{
			if (tile == GC::C_WALL)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
			}
			else if (tile == GC::C_WALL_SIDE_RIGHT)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH - GC::C_OFFSET), collisionPoint.y * GC::TILE_SIZE, GC::WALL_SIDE_WIDTH - GC::C_OFFSET, GC::TILE_SIZE };
			}
			else if (tile == GC::C_FOUNTAIN_BASIN)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::FOUNTAIN_BASIN_HEIGHT + GC::C_OFFSET };
			}
			else if (tile == GC::C_COLUMN_BASE)
			{
				tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, collisionPoint.y * GC::TILE_SIZE, GC::TILE_SIZE - GC::C_OFFSET, GC::COLUMN_BASE_HEIGHT + GC::C_OFFSET };
			}

			if (collisionBox.intersects(tileRect, intersection))
			{
				collided = true;
				frameMovementVector.y = 0;
			}
		}
	}

	return collided;
}

bool CheckCollisionDown(const GameData& game, const sf::FloatRect& collisionBox, Dim2Df& frameMovementVector, const float& wallSideWidth)
{
	sf::FloatRect tileRect = { 0, 0, 0, 0 }; //Map collision rectangle
	sf::FloatRect intersection = { 0, 0, 0, 0 }; //Intersection rectangle
	Dim2Di collisionPointInt = {}; //Which tile the sprite will be on after movement
	Dim2Df collisionPoint = { 0.f, 0.f }; //Which tile the sprite will be on after movement
	int tile = 0; //Collision ID of the tile at the collisionPoint
	bool collided = false; //If the entity has collided with the map

	collisionPointInt = { (int)floor(collisionBox.left + collisionBox.width - GC::C_OFFSET) / GC::TILE_SIZE, (int)floor(collisionBox.top + collisionBox.height + frameMovementVector.y) / GC::TILE_SIZE }; //Bottom right
	collisionPoint = Dim2Df(collisionPointInt);
	tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

	if (tile != GC::C_FREE_MOVEMENT)
	{
		if (tile == GC::C_WALL)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (collisionPoint.y * GC::TILE_SIZE) - GC::C_OFFSET, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
		}
		else if ((tile == GC::C_WALL_TOP) || (tile == GC::C_CORNER_BOTTOM_RIGHT) || (tile == GC::C_CORNER_BOTTOM_LEFT))
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::WALL_TOP_HEIGHT + GC::C_OFFSET };
		}
		else if (tile == GC::C_WALL_TOP_BOTTOM_RIGHT)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_SIDE_WIDTH - GC::C_OFFSET), (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET),
				GC::WALL_SIDE_WIDTH - GC::C_OFFSET, GC::WALL_TOP_HEIGHT + GC::C_OFFSET };
		}
		else if (tile == GC::C_FOUNTAIN_TOP)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET };
		}
		else if (tile == GC::C_COLUMN_TOP)
		{
			tileRect = { (collisionPoint.x * GC::TILE_SIZE) + GC::C_OFFSET, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET };
		}

		if (collisionBox.intersects(tileRect, intersection))
		{
			collided = true;
			frameMovementVector.y = 0;
		}
	}

	if (!collided)
	{
		collisionPointInt = { (int)floor(collisionBox.left + GC::C_OFFSET) / GC::TILE_SIZE, (int)floor(collisionBox.top + collisionBox.height + frameMovementVector.y) / GC::TILE_SIZE }; //Bottom left
		collisionPoint = Dim2Df(collisionPointInt);
		tile = game.collisionMap[collisionPointInt.y][collisionPointInt.x];

		if (tile != GC::C_FREE_MOVEMENT)
		{
			if (tile == GC::C_WALL)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) - GC::C_OFFSET, GC::TILE_SIZE - GC::C_OFFSET, GC::TILE_SIZE + GC::C_OFFSET };
			}
			else if ((tile == GC::C_WALL_TOP) || (tile == GC::C_CORNER_BOTTOM_LEFT) || (tile == GC::C_CORNER_BOTTOM_RIGHT))
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, GC::WALL_TOP_HEIGHT + GC::C_OFFSET };
			}
			else if (tile == GC::C_WALL_TOP_BOTTOM_LEFT)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::WALL_TOP_HEIGHT + GC::C_OFFSET),
					GC::WALL_SIDE_WIDTH - GC::C_OFFSET, (GC::WALL_TOP_HEIGHT + GC::C_OFFSET) };
			}
			else if (tile == GC::C_FOUNTAIN_TOP)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, (GC::FOUNTAIN_TOP_HEIGHT + GC::C_OFFSET) };
			}
			else if (tile == GC::C_COLUMN_TOP)
			{
				tileRect = { collisionPoint.x * GC::TILE_SIZE, (collisionPoint.y * GC::TILE_SIZE) + GC::TILE_SIZE - (GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET), GC::TILE_SIZE - GC::C_OFFSET, (GC::COLUMN_TOP_HEIGHT + GC::C_OFFSET) };
			}

			if (collisionBox.intersects(tileRect, intersection))
			{
				collided = true;
				frameMovementVector.y = 0;
			}
		}
	}

	return collided;
}

//Stops entity body attacks or projectiles if they have collided with the map
void StopMotion(const int& type, Weapon& weapon, Projectile& projectile, const bool& collided)
{
	if (collided)
	{
		if (type == GC::C_ENTITY_BODY_ATTACK)
		{
			//Only stop line or circular motions
			if (weapon.attack0.active)
			{
				if (weapon.attack0.motions[0].active && (weapon.attack0.motions[0].line || weapon.attack0.motions[0].circular))
				{
					weapon.StopAttackIfTrue(collided);
				}
				else if (weapon.attack0.motions[1].active && (weapon.attack0.motions[1].line || weapon.attack0.motions[1].circular))
				{
					weapon.StopAttackIfTrue(collided);
				}
			}
			else if (weapon.attack1.active)
			{
				if (weapon.attack1.motions[0].active && (weapon.attack1.motions[0].line || weapon.attack1.motions[0].circular))
				{
					weapon.StopAttackIfTrue(collided);
				}
				else if (weapon.attack1.motions[1].active && (weapon.attack1.motions[1].line || weapon.attack1.motions[1].circular))
				{
					weapon.StopAttackIfTrue(collided);
				}
			}
		}
		else if (type == GC::C_PROJECTILE)
		{
			projectile.active = false;
		}
	}
}

void CheckMapCollision(const GameData& game, const int& type, sf::Sprite& sprite, Weapon& weapon, Dim2Df& frameMovementVector, Projectile& projectile)
{
	bool movingLeft = false, movingRight = false, movingUp = false, movingDown = false;
	GetMovementBools(type, weapon, frameMovementVector, projectile.angle, movingLeft, movingRight, movingUp, movingDown);

	//Initialize collision rectangles, left and right rects based on the direction the entity is moving
	//example: When facing the south/down direction, left is south east and right is south west
	sf::FloatRect collisionBox = sprite.getGlobalBounds(); //Entity's collision rectangle
	float wallSideWidth = (float)GC::WALL_SIDE_WIDTH; //Modified wall side width

	//Enlarge collision boxes for attacks 
	if (type == GC::C_ENTITY_BODY_ATTACK)
	{
		wallSideWidth *= GC::ENEMY_ATTACK_C_OFFSET;
	}
	//else
	//{
	//	collisionBox.top += collisionBox.height - GC::FEET_COLLISION_HEIGHT;
	//	collisionBox.height = GC::FEET_COLLISION_HEIGHT;
	//}
	collisionBox.top += collisionBox.height - GC::FEET_COLLISION_HEIGHT;
	collisionBox.height = GC::FEET_COLLISION_HEIGHT;

	//Each of the functions below finds collision rectangles of specific tiles and offset them slightly for smooth wall sliding
	//Increases map collision rect in the direction of movement and decrease in the perpendicular direction for better wall sliding
	bool collided = false;

	if (movingUp)
	{
		collided = CheckCollisionUp(game, collisionBox, frameMovementVector, wallSideWidth);
	}
	else if (movingDown)
	{
		collided = CheckCollisionDown(game, collisionBox, frameMovementVector, wallSideWidth);
	}

	if (collided && (type == GC::C_ENTITY_BODY_ATTACK || type == GC::C_PROJECTILE))
	{
		StopMotion(type, weapon, projectile, collided);
	}
	else
	{
		collided = false;

		if (movingLeft)
		{
			collided = CheckCollisionLeft(game, collisionBox, frameMovementVector, wallSideWidth);
		}
		else if (movingRight)
		{
			collided = CheckCollisionRight(game, collisionBox, frameMovementVector, wallSideWidth);
		}

		StopMotion(type, weapon, projectile, collided);
	}
}