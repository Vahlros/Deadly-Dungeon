#include "rooms.h"
#include <string>

//Came from help from the SFML discord server, centres text origin
void CentreTextOrigin(sf::Text& text)
{
	const auto localBounds = sf::Vector2f(text.getLocalBounds().left, text.getLocalBounds().top);
	const auto globalOrigin = sf::Vector2f(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);

	text.setOrigin(localBounds + globalOrigin);
}

void Shop::SetupShop(const GameData& game, const char& ID)
{
	//Assign shop values
	name.setFont(game.font);
	name.setCharacterSize(GC::TEXT_CHARACTER_SIZE);
	sprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);

	switch (ID)
	{
	case GC::WS_HEALTH:
		itemID = ID;
		price = 10;
		itemName = "Max Health: ";
		sprite.setTextureRect(GC::POTION_RECT_R1);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::WS_SPEED:
		itemID = ID;
		price = 15;
		itemName = "Speed: ";
		sprite.setTextureRect(GC::POTION_RECT_Y1);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::WS_POWER:
		itemID = ID;
		price = 15;
		itemName = "Attack Power: ";
		sprite.setTextureRect(GC::POTION_RECT_G1);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::WS_ATTACK_SPEED:
		itemID = ID;
		price = 10;
		itemName = "Attack Speed: ";
		sprite.setTextureRect(GC::POTION_RECT_G2);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::WS_KNOCKBACK:
		itemID = ID;
		price = 15;
		itemName = "Knockback Power: ";
		sprite.setTextureRect(GC::POTION_RECT_B1);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::WS_FULL_HEAL:
		itemID = ID;
		price = 15;
		itemName = "Full Heal: ";
		unlimitedItems = true;
		sprite.setTextureRect(GC::POTION_RECT_R2);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::LS_FANCY_SWORD:
		itemID = ID;
		price = 10;
		itemName = "Fancy Sword: ";
		unlimitedItems = true;
		sprite.setTextureRect(GC::F_SWORD_RECT);
		sprite.setOrigin(GC::F_SWORD_ORIGIN);
		break;

	case GC::LS_SPEAR:
		itemID = ID;
		price = 10;
		itemName = "Spear: ";
		unlimitedItems = true;
		sprite.setTextureRect(GC::SPEAR_RECT);
		sprite.setOrigin(GC::SPEAR_ORIGIN);
		break;

	case GC::LS_BIG_WEAPONS:
		itemID = ID;
		price = 20;
		itemName = "Enlarge Weapons: ";
		sprite.setTextureRect(GC::POTION_RECT_B2);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	case GC::LS_MELEE_PROJECTILE:
		itemID = ID;
		price = 20;
		itemName = "Melee Bullets: ";
		sprite.setTextureRect(GC::POTION_RECT_Y2);
		sprite.setOrigin(GC::POTION_ORIGIN);
		break;

	default:
		printf("Item ID not recognised, shop not initialized properly\n");
		break;
	}

	itemValue = std::to_string(price);
	name.setString(itemName + itemValue);
	CentreTextOrigin(name);
	name.setPosition(position);
	sprite.setPosition({ position.x, position.y - GC::TILE_SIZE });
}

void Shop::Render(sf::RenderWindow& window)
{
	window.draw(name);

	if (!sold)
	{
		window.draw(sprite);
	}
}

char Shop::Buy(GameData& game, short& coins)
{
	if (coins >= price)
	{
		coins -= price;
		game.metrics.UpdatePurchases(itemID, (unsigned char)price);

		if (!unlimitedItems)
		{
			sold = true;
			name.setString("Sold out!");
			CentreTextOrigin(name);
			name.setPosition(position);
		}
		else
		{
			IncreasePrice();
		}

		return itemID;
	}

	return -1;
}

void Shop::IncreasePrice()
{
	//Set new price
	price += GC::UNLIMITED_ITEM_PRICE_INCREASE;
	itemValue = std::to_string(price);
	name.setString(itemName + itemValue);
}

void Room::Init(GameData& game, const int& roomNumber, const Dim2Di& position)
{
	ID = roomNumber;
	data = &GC::ROOM_LIST[ID];
	spawners.resize(GC::MAX_SPAWNERS);
	shops.resize(GC::MAX_SHOPS);

	//Position
	rect.left = position.x;
	rect.top = position.y;

	//Dimensions
	GetTypeDimensions(rect.width, rect.height);



	//Draw tiles
	unsigned char tileID = 0, tileAboveID = 1;
	for (char y = 0; y < rect.height; ++y)
	{
		for (char x = 0; x < rect.width; ++x)
		{
			tileID = data->tilemap[y][x];

			if (y > 0)
			{
				tileAboveID = data->tilemap[y - 1][x];
			}

			if (tileID < GC::DOOR_START_NUM)
			{
				TileDrawing(game, x, y, tileID, tileAboveID);
				AlterCollisionMap(game, x, y, tileID);
				CheckForAnimatedTiles(game, x, y, tileID);
				CheckForSpawner(x, y, tileID);
				CheckForShop(x, y, tileID);
			}
		}
	}

	//Adjust spawner vector to actual size
	spawners.resize(spawnerCounter);
}

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

void Room::TileDrawing(GameData& game, const int& x, const int& y, const char& tileID, const char& tileAboveID)
{
	//Complicated alterations based on tile ID
	bool requiresFloor = false, wallside = false, walltop = false, corner = false, right = false, smallWalltop = false;
	char correction = 0;
	Tile tile = GC::TILE_LIST[tileID];

	TileDrawingBools(tile, requiresFloor, wallside, walltop, corner, right, smallWalltop);

	if (requiresFloor && (wallside || (y != 0)))
	{
		//Check if random floor should be drawn
		if (!(walltop && (tileAboveID == GC::T_DARKNESS)))
		{
			DrawRandomFloor(game, x, y);
		}

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
		else if (smallWalltop)
		{
			correction = GC::TILE_SIZE - GC::WALL_TOP_HEIGHT;
			char correction2 = 0;

			if (right)
			{
				correction2 = GC::TILE_SIZE - GC::WALL_SIDE_WIDTH;
			}

			game.textures[GC::WALL_SIDE_TOP_TEXTURE].loadFromImage(game.spritesheetImg, tile.rect);

			game.textures[GC::MAP_FLOOR_TEXTURE].update(game.textures[GC::WALL_SIDE_TOP_TEXTURE], ((rect.left + x) * GC::TILE_SIZE) + correction2, ((rect.top + y) * GC::TILE_SIZE) + correction);
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

void Room::TileDrawingBools(const Tile& tile, bool& requiresFloor, bool& wallside, bool& walltop, bool& corner, bool& right, bool& smallWalltop)
{
	if ((tile.ID >= GC::WALL_TOP_RANGE.x) && (tile.ID <= GC::WALL_TOP_RANGE.y))
	{
		requiresFloor = true;
		walltop = true;
	}
	else if ((tile.ID >= GC::WALL_SIDE_RANGE.x) && (tile.ID <= GC::WALL_SIDE_RANGE.y))
	{
		requiresFloor = true;

		if (tile.ID == GC::T_WALL_SIDE_TOP_LEFT)
		{
			smallWalltop = true;
		}
		else if (tile.ID == GC::T_WALL_SIDE_TOP_RIGHT)
		{
			smallWalltop = true;
			right = true;
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
		else
		{
			game.collisionMap[tileY][tileX] = GC::C_FREE_MOVEMENT;
		}
	}
	else
	{
		game.collisionMap[tileY][tileX] = GC::C_WALL;
	}
}

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

void Room::CheckForSpawner(const int& x, const int& y, const char& tileID)
{
	if (tileID == GC::T_ENEMY_SPAWNER)
	{
		Dim2Df position;
		position.x = ((float)(rect.left + x) + 0.5f) * GC::TILE_SIZE;
		position.y = ((float)(rect.top + y) + 0.5f) * GC::TILE_SIZE;

		spawners[spawnerCounter] = position;
		spawnerCounter += 1;
	}
}

void Room::CheckForShop(const int& x, const int& y, const char& tileID)
{
	bool foundShop = false;

	if (tileID == GC::T_WATER_SHOP)
	{
		foundShop = true;
		shops[shopCounter].water = true;
	}
	else if (tileID == GC::T_LAVA_SHOP)
	{
		foundShop = true;
		shops[shopCounter].water = false;
	}

	if (foundShop)
	{
		Dim2Df position;
		position.x = (float)(rect.left + x + 0.5f) * GC::TILE_SIZE;
		position.y = (float)(rect.top + y + 0.5f) * GC::TILE_SIZE;

		shops[shopCounter].position = position;
		shopCounter += 1;
	}
}

void Room::RenderShops(sf::RenderWindow& window)
{
	for (unsigned char index = 0; index < shops.size(); index++)
	{
		shops[index].Render(window);
	}
}