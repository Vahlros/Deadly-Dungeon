#include "rooms.h"

//Initializes the tilemap editor
void Room::Init(GameData& game, const int& roomNumber, const Dim2Di& position)
{
	data = &GC::ROOM_LIST[roomNumber];

	//Position
	rect.left = position.x;
	rect.top = position.y;

	//Dimensions
	GetTypeDimensions(rect.width, rect.height);

	//Draw tiles
	unsigned char tileID;
	for (char y = 0; y < rect.height; ++y)
	{
		for (char x = 0; x < rect.width; ++x)
		{
			tileID = data->tilemap[y][x];
			if (tileID < GC::DOOR_START_NUM)
			{
				TileDrawing(game, x, y, tileID);
				AlterCollisionMap(game, x, y, tileID);
				CheckForAnimatedTiles(game, x, y, tileID);
			}
		}
	}

	//Draw doors
	FindDoors();
	unsigned char doorID;

	for (char z = 0; z < doorCounter; ++z)
	{
		doorID = data->tilemap[doorsList[z].top][doorsList[z].left];
		game.textures[GC::DOOR_TEXTURE].loadFromImage(game.spritesheetImg, GC::TILE_LIST[doorID].rect);
		game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::DOOR_TEXTURE], (rect.left + doorsList[z].left) * GC::TILE_SIZE, (rect.top + doorsList[z].top) * GC::TILE_SIZE);
	}
}

//Finds doors in this room
void Room::FindDoors()
{
	bool firstDoor = true;

	for (char y = 0; y < rect.height; ++y)
	{
		for (char x = 0; x < rect.width; ++x)
		{
			if (data->tilemap[y][x] >= GC::DOOR_START_NUM)
			{
				if (firstDoor)
				{
					doorsList[doorCounter] = { x, y, GC::DOOR_TILE_WIDTH, GC::DOOR_TILE_HEIGHT };
					doorCounter += 1;
					firstDoor = false;
				}
				else if (!CheckIfFoundDoor(doorsList, doorCounter, x, y))
				{
					doorsList[doorCounter] = { x, y, GC::DOOR_TILE_WIDTH, GC::DOOR_TILE_HEIGHT };
					doorCounter += 1;
				}
			}
		}
	}
}

//Check if this door tile is part of an already found door
bool Room::CheckIfFoundDoor(std::vector<sf::IntRect>& doorsList, const int& doorCounter, const int& x, const int& y)
{
	bool found = false;
	for (int z = 0; z < doorCounter; ++z)
	{
		if ((x >= doorsList[z].left) && (x < (doorsList[z].left + doorsList[z].width)))
		{
			if ((y >= doorsList[z].top) && (y < (doorsList[z].top + doorsList[z].height)))
			{
				found = true;
			}
		}
	}
	return found;
}

//Changes variables to dimensions of the room
void Room::GetTypeDimensions(int& width, int& height)
{
	if (data->type == GC::R32X32)
	{
		width = GC::ROOM_TYPE_0.x;
		height = GC::ROOM_TYPE_0.y;
	}
	else if (data->type == GC::R16X16)
	{
		width = GC::ROOM_TYPE_1.x;
		height = GC::ROOM_TYPE_1.y;
	}
	else if (data->type == GC::R32X16)
	{
		width = GC::ROOM_TYPE_2.x;
		height = GC::ROOM_TYPE_2.y;
	}
	else if (data->type == GC::R16X32)
	{
		width = GC::ROOM_TYPE_3.x;
		height = GC::ROOM_TYPE_3.y;
	}
}

//Updates the 
void Room::TileDrawing(GameData& game, const int& x, const int& y, const char& tileID)
{
	//Complicated alterations based on tile ID
	bool requiresFloor = false, wallside = false, walltop = false, corner = false, right = false;
	char correction = 0;
	Tile tile = GC::TILE_LIST[tileID];

	TileDrawingBools(tile, requiresFloor, wallside, walltop, corner, right);

	if (requiresFloor && (y != 0))
	{
		DrawRandomFloor(game, x, y);

		//Draw tile
		if (wallside)
		{
			tile.rect.width = GC::WALL_SIDE_WIDTH;

			if (right)
			{
				correction = GC::TILE_SIZE - GC::WALL_SIDE_WIDTH;

				tile.rect.left += correction;
				game.textures[GC::WALL_SIDE_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);

				tile.rect.left -= correction;
				game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_SIDE_TEXTURE], ((rect.left + x) * GC::TILE_SIZE) + correction, (rect.top + y) * GC::TILE_SIZE);
			}
			else
			{
				game.textures[GC::WALL_SIDE_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);
				game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_SIDE_TEXTURE], (rect.left + x) * GC::TILE_SIZE, (rect.top + y) * GC::TILE_SIZE);
			}
		}
		else if (walltop)
		{
			tile.rect.height = GC::WALL_TOP_HEIGHT;
			correction = GC::TILE_SIZE - GC::WALL_TOP_HEIGHT;

			tile.rect.top += correction;
			game.textures[GC::WALL_TOP_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);

			tile.rect.top -= correction;
			game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_TOP_TEXTURE], (rect.left + x) * GC::TILE_SIZE, ((rect.top + y) * GC::TILE_SIZE) + correction);
		}
		else if (corner)
		{
			//Wall top
			tile.rect.height = GC::WALL_TOP_HEIGHT;
			correction = GC::TILE_SIZE - GC::WALL_TOP_HEIGHT;

			tile.rect.top += correction;
			game.textures[GC::WALL_TOP_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);

			tile.rect.top -= correction;
			game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_TOP_TEXTURE], (rect.left + x) * GC::TILE_SIZE, ((rect.top + y) * GC::TILE_SIZE) + correction);

			//Wall side
			tile.rect.height = GC::TILE_SIZE;
			tile.rect.width = GC::WALL_SIDE_WIDTH;

			if (right)
			{
				correction = GC::TILE_SIZE - GC::WALL_SIDE_WIDTH;

				tile.rect.left += correction;
				game.textures[GC::WALL_SIDE_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);

				tile.rect.left -= correction;
				game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_SIDE_TEXTURE], ((rect.left + x) * GC::TILE_SIZE) + correction, (rect.top + y) * GC::TILE_SIZE);
			}
			else
			{
				game.textures[GC::WALL_SIDE_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);
				game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_SIDE_TEXTURE], (rect.left + x) * GC::TILE_SIZE, (rect.top + y) * GC::TILE_SIZE);
			}
		}
		else
		{
			game.textures[GC::TILE_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);
			game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::TILE_TEXTURE], (rect.left + x) * GC::TILE_SIZE, (rect.top + y) * GC::TILE_SIZE);
		}
	}
	else //No extra drawing required for this tile
	{
		game.textures[GC::TILE_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);
		game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::TILE_TEXTURE], (rect.left + x) * GC::TILE_SIZE, (rect.top + y) * GC::TILE_SIZE);
	}
}

//Gets booleans for current tile
void Room::TileDrawingBools(const Tile& tile, bool& requiresFloor, bool& wallside, bool& walltop, bool& corner, bool& right)
{
	if ((tile.ID >= GC::WALL_TOP_RANGE.x) && (tile.ID <= GC::WALL_TOP_RANGE.y))
	{
		requiresFloor = true;
		walltop = true;
	}
	else if ((tile.ID >= GC::WALL_SIDE_RANGE.x) && (tile.ID <= GC::WALL_SIDE_RANGE.y))
	{
		requiresFloor = true;

		if ((tile.ID == GC::T_WALL_SIDE_TOP_LEFT) || (tile.ID == GC::T_WALL_SIDE_TOP_RIGHT))
		{
			walltop = true;
		}
		else if ((tile.ID == GC::T_WALL_SIDE_MID_RIGHT) || (tile.ID == GC::T_WALL_SIDE_FRONT_RIGHT))
		{
			wallside = true;
			right = true;
		}
		else
		{
			wallside = true;
		}
	}
	else if ((tile.ID >= GC::WALL_CORNER_RANGE.x) && (tile.ID <= GC::WALL_CORNER_RANGE.y))
	{
		requiresFloor = true;
		corner = true;

		if ((tile.ID == GC::T_WALL_CORNER_BOTTOM_RIGHT) || (tile.ID == GC::T_WALL_IN_CORNER_L_TOP_RIGHT))
		{
			right = true;
		}
	}
}

//Draws a random floor at a specific locations
void Room::DrawRandomFloor(GameData& game, const int& x, const int& y)
{
	//Get random floor, approx 10% chance to be a blemished floor, 90% to be Floor 1
	int floorID = std::rand() % (GC::FLOOR_RANGE.y * 2);

	if (floorID < GC::FLOOR_RANGE.x)
	{
		floorID = GC::FLOOR_RANGE.x;
	}
	else if (floorID > GC::FLOOR_RANGE.y)
	{
		floorID = GC::FLOOR_RANGE.x;
	}

	//Draw random floor
	game.textures[GC::TILE_TEXTURE].loadFromImage(game.spritesheetImg, GC::TILE_LIST[floorID].rect);
	game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::TILE_TEXTURE], (rect.left + x) * GC::TILE_SIZE, (rect.top + y) * GC::TILE_SIZE);
}

//Alters the collision map based on the tile
void Room::AlterCollisionMap(GameData& game, const int& x, const int& y, const char& tileID) //NEEDS EDITING
{
	unsigned char tileX = x + rect.left, tileY = y + rect.top;

	if (tileID > GC::WALL_RANGE.y)
	{
		if (tileID == GC::T_WALL_COLUMN_BASE) //Column bases
		{
			game.collisionMap[tileY][tileX] = GC::C_COLUMN_BASE;
		}
		else if ((tileID == GC::T_WATER_FOUNTAIN_BASIN) || (tileID == GC::T_LAVA_FOUNTAIN_BASIN)) //Fountain basins
		{
			game.collisionMap[tileY][tileX] = GC::C_FOUNTAIN_BASIN;
		}
		else if (tileID == GC::T_WALL_COLUMN_TOP_FLOOR) //Column bases
		{
			game.collisionMap[tileY][tileX] = GC::C_COLUMN_TOP;
		}
		else if (tileID == GC::T_FOUNTAIN_TOP_FLOOR) //Column bases
		{
			game.collisionMap[tileY][tileX] = GC::C_FOUNTAIN_TOP;
		}
		else if (tileID < GC::WALL_SIDE_RANGE.x) //Floors
		{
			game.collisionMap[tileY][tileX] = GC::C_FREE_MOVEMENT;
		}
		else if (tileID == GC::T_WALL_SIDE_TOP_LEFT) //Small wall top left
		{
			game.collisionMap[tileY][tileX] = GC::C_WALL_TOP_BOTTOM_LEFT;
		}
		else if (tileID == GC::T_WALL_SIDE_TOP_RIGHT) //Small wall top right
		{
			game.collisionMap[tileY][tileX] = GC::C_WALL_TOP_BOTTOM_RIGHT;
		}
		else if (tileID <= GC::WALL_SIDE_RANGE.y - 2) //Wall sides right
		{
			game.collisionMap[tileY][tileX] = GC::C_WALL_SIDE_RIGHT;
		}
		else if (tileID <= GC::WALL_SIDE_RANGE.y) //Wall sides left
		{
			game.collisionMap[tileY][tileX] = GC::C_WALL_SIDE_LEFT;
		}
		else if (tileID <= GC::WALL_TOP_RANGE.y) //Wall tops
		{
			game.collisionMap[tileY][tileX] = GC::C_WALL_TOP;
		}
		else if (tileID <= GC::WALL_CORNER_RANGE.y - 2) //Wall corners bottom left
		{
			game.collisionMap[tileY][tileX] = GC::C_CORNER_BOTTOM_LEFT;
		}
		else if (tileID <= GC::WALL_CORNER_RANGE.y) //Wall corner bottoms right
		{
			game.collisionMap[tileY][tileX] = GC::C_CORNER_BOTTOM_RIGHT;
		}
		else //Doors
		{
			game.collisionMap[tileY][tileX] = GC::C_FREE_MOVEMENT;
		}
	}
	else
	{
		game.collisionMap[tileY][tileX] = GC::C_WALL;
	}
}

//Checks for animated tiles
void Room::CheckForAnimatedTiles(GameData& game, const int& x, const int& y, const char& tileID)
{
	if (tileID == 18) //Lava fountain
	{
		AnimatedTiles animTile;

		animTile.sprite.setTexture(game.textures[GC::LAVA_FOUNTAIN_TEXTURE]);
		animTile.sprite.setTextureRect({ 0, 0, GC::TILE_SIZE, GC::FOUNTAIN_ANIM_LAVA_RECT.height });
		animTile.sprite.setPosition((float)((rect.left + x) * GC::TILE_SIZE),
									(float)(((rect.top + y) * GC::TILE_SIZE) - GC::FOUNTAIN_ANIM_TOP_RECT.height));
		animTile.anim = { &GC::FOUNTAIN_ANIM_LAVA, 0, 0.f };

		animatedTiles.push_back(animTile);
	}
	else if (tileID == 19) //Water fountain
	{
		AnimatedTiles animTile;

		animTile.sprite.setTexture(game.textures[GC::WATER_FOUNTAIN_TEXTURE]);
		animTile.sprite.setTextureRect({ 0, 0, GC::TILE_SIZE, GC::FOUNTAIN_ANIM_WATER_RECT.height });
		animTile.sprite.setPosition((float)((rect.left + x) * GC::TILE_SIZE),
									(float)(((rect.top + y) * GC::TILE_SIZE) - GC::FOUNTAIN_ANIM_TOP_RECT.height));
		animTile.anim = { &GC::FOUNTAIN_ANIM_WATER, 0, 0.f };

		animatedTiles.push_back(animTile);
	}
}

//Updates animated tiles
void Room::UpdateAnimatedTiles(const GameData& game, sf::RenderWindow& window)
{
	bool animVisible = false;

	for (int x = 0; x != animatedTiles.size(); ++x)
	{
		animVisible = true;

		if (animVisible)
		{
			animatedTiles[x].anim.UpdateAnimation(animatedTiles[x].sprite, game.elapsed);
			window.draw(animatedTiles[x].sprite);
			animVisible = false;
		}
	}
}

//Finds enemy spawn locations
void Room::FindSpawners()
{

}