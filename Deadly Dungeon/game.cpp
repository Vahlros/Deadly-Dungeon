#include "game.h"
#include "maths.h"

//Initializes game session
void Game::Init(sf::RenderWindow& window)
{
	//Random
	srand((int)time(0)); //Sets random's seed to current time, for "true random"

	//Game data
	data.Init(window);

	//Rooms
	roomList.resize(GC::ROOM_NUM);
	roomList[0].Init(data, 0, { 32, 32 });
	roomList[1].Init(data, 1, { 64, 32 });
	roomList[2].Init(data, 2, { 64, 64 });
	roomList[3].Init(data, 3, { 32, 64 });

	InitShops(data, roomList);

	//Player
	player1.Init(data, GetRandomSpawn(roomList, true, 0));
	data.playerHit = &player1.hit;

	//Enemy
	enemyList.resize(GC::MAX_ENEMIES);
	enemyList[0].ID = 1;
	enemyList[0].Init(data, { 732.f, 732.f });

	//Projectiles
	projectiles.resize(GC::MAX_PROJECTILES);
	InitProjectiles(data, projectiles);
}

//Main game loop
void Game::GameLoop(sf::RenderWindow& window)
{
	//Clock
	data.elapsed = clock.getElapsedTime().asSeconds();
	if (data.elapsed > GC::APPROX_ELAPSED)
	{
		data.elapsed = GC::APPROX_ELAPSED; //Clamp elapsed time, for testing purposes
	}
	clock.restart();

	//Player input
	player1.Update(window, data, projectiles, enemyList);

	//Enemy
	enemyList[0].Update(data, projectiles, player1.entity);

	//Update the window
	data.RenderMap(window, player1.entity.sprite.getPosition());

	//Render rooms
	RenderMapNearPlayer(data, window, roomList, player1.entity.sprite);

	//Render enemies
	if (enemyList[0].active)
	{
		enemyList[0].entity.Render(window, data);
	}

	//Render Projectiles
	UpdateProjectiles(data, window, projectiles, enemyList, player1);

	//Render player last
	player1.entity.Render(window, data);
}

//Check player death
void Game::IsPlayerDead()
{
	if (data.playerDead)
	{
		//END THE GAME SESSION
	}
}

//Initializes all projectiles
void InitProjectiles(const GameData& game, std::vector<Projectile>& projList)
{
	for (short index = 0; index < GC::MAX_PROJECTILES; ++index)
	{
		projList[index].sprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);
	}
}

//Updates all active projectiles
void UpdateProjectiles(const GameData& game, sf::RenderWindow& window, std::vector<Projectile>& projList, std::vector<Enemy>& enemies, Player& player)
{
	for (short index = 0; index < GC::MAX_PROJECTILES; ++index)
	{
		if (projList[index].active)
		{
			projList[index].Update(game);
			CheckProjectileCollision(projList[index], enemies, player);
			projList[index].Render(window);
		}
	}
}

//Projectile collision detection
void CheckProjectileCollision(Projectile& proj, std::vector<Enemy>& enemies, Player& player)
{
	if (proj.playerProjectile)
	{
		//Modified player attack collision
		for (char index = 0; index < GC::MAX_ENEMIES; index++)
		{
			if (enemies[index].active && !enemies[index].entity.invulnerable)
			{
				//Calculate distance to enemy
				Dim2Df position = enemies[index].entity.sprite.getPosition();
				float distanceToEnemy = CalculateMagnitudeOfVector(proj.sprite.getPosition() - position);

				//If in range, check collision
				if (distanceToEnemy <= GC::CHECK_ATTACK_COLLISION_RANGE)
				{
					if (proj.sprite.getGlobalBounds().intersects(enemies[index].entity.sprite.getGlobalBounds()))
					{
						//Hit enemy
						enemies[index].entity.TakeDamage(GC::DEFAULT_DAMAGE, player.entity.facing, player.knockbackPower);

						//Deactivate projectile
						proj.active = false;
					}
				}
			}
		}
	}
	else
	{
		if (!player.entity.invulnerable)
		{
			//Calculate distance to player
			Dim2Df position = player.entity.sprite.getPosition();
			float distanceToEnemy = CalculateMagnitudeOfVector(proj.sprite.getPosition() - position);

			//If in range, check collision
			if (distanceToEnemy <= GC::CHECK_ATTACK_COLLISION_RANGE)
			{
				if (proj.sprite.getGlobalBounds().intersects(player.entity.sprite.getGlobalBounds()))
				{
					//Hit player
					player.entity.TakeDamage(GC::DEFAULT_DAMAGE, DirectionalAngle{}, GC::ZERO);
					player.entity.invulnerable = true;
					player.hit = true;

					//Deactivate projectile
					proj.active = false;
				}
			}
		}
	}
}

//Randomly initializes shops
void InitShops(const GameData& game, std::vector<Room>& rooms)
{
	//Create item vectors
	std::vector<char> waterItems(GC::TOTAL_WATER_SHOPS - 1);
	waterItems = { GC::WS_HEALTH, GC::WS_SPEED, GC::WS_POWER, GC::WS_ATTACK_SPEED, GC::WS_KNOCKBACK };
	std::vector<char> lavaItems(GC::TOTAL_LAVA_SHOPS);
	lavaItems = { GC::LS_FANCY_SWORD, GC::LS_SPEAR, GC::LS_BIG_WEAPONS, GC::LS_REFLECT, GC::LS_MELEE_PROJECTILE, GC::LS_MELEE_PROJECTILE, GC::LS_MELEE_PROJECTILE };
	
	//Assign items to shops
	rooms[3].shops[0].SetupShop(game, GC::WS_FULL_HEAL);

	for (char roomID = 0; roomID < GC::ROOM_NUM - 1; roomID++)
	{
		for (char shopID = 0; shopID < GC::MAX_SHOPS; shopID++)
		{
			unsigned int item, index;

			if (rooms[roomID].shops[shopID].water)
			{
				if (waterItems.size() != 1)
				{
					index = rand() % (waterItems.size() - 1);
					item = waterItems[index];
					waterItems.erase(waterItems.begin() + index);
				}
				else
				{
					item = waterItems[0];
				}
			}
			else //Lava
			{
				if (lavaItems.size() != 1)
				{
					index = rand() % (lavaItems.size() - 1);
					item = lavaItems[index];
					lavaItems.erase(lavaItems.begin() + index);
				}
				else
				{
					item = lavaItems[0];
				}
			}

			rooms[roomID].shops[shopID].SetupShop(game, item);
		}
	}
}

//Render shops and fountains near the player
void RenderMapNearPlayer(const GameData& game, sf::RenderWindow& window, std::vector<Room>& rooms, sf::Sprite playerSprite)
{
	bool foundRoom = false;
	char index = 0;

	for (unsigned char index = 0; index < rooms.size(); index++)
	{
		sf::FloatRect roomRect = { (float)rooms[index].rect.left * GC::TILE_SIZE, (float)rooms[index].rect.top * GC::TILE_SIZE,
			(float)rooms[index].rect.width * GC::TILE_SIZE, (float)rooms[index].rect.height * GC::TILE_SIZE };

		if (playerSprite.getGlobalBounds().intersects(roomRect))
		{
			foundRoom = true;
		}

		if (foundRoom)
		{
			for (unsigned char shopIndex = 0; shopIndex < rooms[index].shops.size(); shopIndex++)
			{
				rooms[index].shops[shopIndex].Render(window);
			}

			rooms[index].UpdateAnimatedTiles(game, window);
		}
	}
}

//Find a random spawn position
Dim2Df GetRandomSpawn(std::vector<Room>& rooms, const bool& randRoom, const char& roomID)
{
	unsigned int roomIndex;

	if (randRoom)
	{
		roomIndex = rand() % (GC::ROOM_NUM - 1);
	}
	else
	{
		roomIndex = roomID;
	}

	unsigned int randomSpawner = rand() % (rooms[roomIndex].spawners.size() - 1);

	return rooms[roomIndex].spawners[randomSpawner];
}