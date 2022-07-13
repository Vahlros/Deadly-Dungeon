#include "game.h"
#include "maths.h"

//Initializes all projectiles
void InitProjectiles(const GameData& game, std::vector<Projectile>& projList)
{
	for (short index = 0; index < GC::MAX_PROJECTILES; ++index)
	{
		projList[index].sprite.setTexture(game.textures[GC::SPRITESHEET_TEXTURE]);
	}
}

//Projectile collision detection
void CheckProjectileCollision(GameData& game, Projectile& proj, std::vector<Enemy>& enemies, Player& player)
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
						enemies[index].entity.TakeDamage(GC::DEFAULT_DAMAGE, player.entity.facing, -1.f);

						//Deactivate projectile
						proj.active = false;

						if (enemies[index].entity.isAlive)
						{
							//Interrupt enemy attacks
							if (enemies[index].entity.weapon.attacking)
							{
								enemies[index].AttackCancel();
							}
						}
						else
						{
							//Update metrics
							game.metrics.UpdateKills(GC::ID_PROJECTILE, enemies[index].ID);
							player.EarnCoins(enemies[index].ID);
						}
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

					//Update metrics
					game.metrics.UpdatePlayerDamage(proj.enemyID, GC::DEFAULT_DAMAGE);
				}
			}
		}
	}

	if (proj.active)
	{
		Weapon tempWeapon;
		Dim2Df frameMovementVector = proj.GetFrameMovementVector(game);
		CheckMapCollision(game, GC::C_PROJECTILE, proj.sprite, tempWeapon, frameMovementVector, proj);
	}
}

//Updates all active projectiles
void UpdateProjectiles(GameData& game, std::vector<Projectile>& projList, std::vector<Enemy>& enemies, Player& player)
{
	for (short index = 0; index < GC::MAX_PROJECTILES; ++index)
	{
		if (projList[index].active)
		{
			projList[index].Update(game);
			CheckProjectileCollision(game, projList[index], enemies, player);
		}
	}
}

//Updates all active projectiles
void RenderProjectiles(sf::RenderWindow& window, std::vector<Projectile>& projList)
{
	for (int i = 0; i < GC::MAX_PROJECTILES; i++)
	{
		if (projList[i].active)
		{
			projList[i].Render(window);
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
	lavaItems = { GC::LS_FANCY_SWORD, GC::LS_SPEAR, GC::LS_BIG_WEAPONS, GC::LS_MELEE_PROJECTILE, GC::LS_MELEE_PROJECTILE, GC::LS_MELEE_PROJECTILE, GC::LS_MELEE_PROJECTILE };

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
					index = rand() % waterItems.size();
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
					index = rand() % lavaItems.size();
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
void RenderRooms(const GameData& game, sf::RenderWindow& window, std::vector<Room>& rooms)
{
	for (unsigned char roomIndex = 0; roomIndex < rooms.size(); roomIndex++)
	{
		for (unsigned char shopIndex = 0; shopIndex < rooms[roomIndex].shops.size(); shopIndex++)
		{
			rooms[roomIndex].shops[shopIndex].Render(window);
		}

		rooms[roomIndex].UpdateAnimatedTiles(game, window);
	}
}

//Find a random spawn position
Dim2Df GetRandomSpawn(const std::vector<Room>& rooms, const bool& randRoom, const char& roomID)
{
	unsigned int roomIndex;

	if (randRoom)
	{
		roomIndex = rand() % GC::ROOM_NUM;
	}
	else
	{
		roomIndex = roomID;
	}

	unsigned int randomSpawner = rand() % rooms[roomIndex].spawners.size();

	return rooms[roomIndex].spawners[randomSpawner];
}

//Finds a random spawn position that's not too close to the player
Dim2Df GetValidEnemySpawn(const std::vector<Room>& rooms, const Dim2Df& playerPosition, const char& roomID)
{
	Dim2Df spawnPosition = GetRandomSpawn(rooms, false, roomID);
	float distanceToPlayer;
	bool found = false;
	short attempts = 0;

	while (!found && (attempts != GC::MAX_SPAWN_ATTEMPTS))
	{
		//Ensure spawn is not the same as the player's
		while (spawnPosition == playerPosition)
		{
			spawnPosition = GetRandomSpawn(rooms, false, roomID);
		}

		distanceToPlayer = CalculateMagnitudeOfVector(spawnPosition - playerPosition);

		//Ensure valid distance
		if (distanceToPlayer >= GC::VALID_SPAWN_DISTANCE)
		{
			found = true;
		}

		attempts += 1;
	}

	if (attempts == GC::MAX_SPAWN_ATTEMPTS)
	{
		spawnPosition = GetRandomSpawn(rooms, false, roomID);
	}

	return spawnPosition;
}

//Returns the difficulty rating based on play time
short GetDifficultyRating(const float& totalTime)
{
	float halfMinute = 30.f;
	float difficulty = totalTime / halfMinute;

	return (short)floor(difficulty);
}

////Creates an enemy wave based on difficulty, returns number of enemies
//char CreateEnemyWave(std::vector<Enemy>& enemies, const short& difficulty)
//{
//	char enemiesCreated = 0;
//	enemies[0].ID = 0;
//	enemies[1].ID = 0;
//
//	switch (difficulty)
//	//switch (11) //For testing purposes
//	{
//	case GC::D_TRIVIAL:
//		enemiesCreated = 2;
//		break;
//
//	case GC::D_WALK_IN_THE_PARK:
//		enemies[2].ID = rand() % 2;
//		enemiesCreated = 3;
//		break;
//
//	case GC::D_VERY_EASY:
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = 0;
//		enemiesCreated = 4;
//		break;
//
//	case GC::D_EASY:
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = rand() % 2;
//		enemiesCreated = 4;
//		break;
//
//	case GC::D_KINDA_EASY:
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = rand() % 2;
//		enemies[4].ID = 0;
//		enemiesCreated = 5;
//		break;
//
//	case GC::D_NORMAL:
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = (rand() % 2) + 1;
//		enemies[4].ID = 0;
//		enemiesCreated = 5;
//		break;
//
//	case GC::D_KINDA_HARD:
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = rand() % 2;
//		enemies[4].ID = (rand() % 2) + 1;
//		enemiesCreated = 5;
//		break;
//
//	case GC::D_HARD:
//		enemies[2].ID = (rand() % 2) + 1;
//		enemies[3].ID = (rand() % 2) + 1;
//		enemies[4].ID = (rand() % 2) + 1;
//		enemiesCreated = 5;
//		break;
//
//	case GC::D_VERY_HARD:
//		enemies[2].ID = (rand() % 2) + 1;
//		enemies[3].ID = (rand() % 2) + 1;
//		enemies[4].ID = (rand() % 2) + 1;
//		enemies[5].ID = rand() % 2;
//		enemiesCreated = 6;
//		break;
//
//	case GC::D_BRUTAL:
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = (rand() % 2) + 1;
//		enemies[4].ID = (rand() % 2) + 1;
//		enemies[5].ID = 3;
//		enemiesCreated = 6;
//		break;
//
//	case GC::D_NIGHTMARE:
//		enemies[0].ID = rand() % 2;
//		enemies[1].ID = rand() % 2;
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = (rand() % 2) + 1;
//		enemies[4].ID = (rand() % 2) + 1;
//		enemies[5].ID = 3;
//		enemies[6].ID = 3;
//		enemiesCreated = 7;
//		break;
//
//	default:
//		enemies[0].ID = rand() % 2;
//		enemies[1].ID = rand() % 2;
//		enemies[2].ID = rand() % 2;
//		enemies[3].ID = (rand() % 2) + 1;
//		enemies[4].ID = (rand() % 2) + 1;
//		enemies[5].ID = 3;
//		enemies[6].ID = 3;
//		enemiesCreated = 7;
//	}
//
//	return enemiesCreated;
//}

void BoostWave(std::vector<Enemy>& enemies, const int& difficulty)
{
	//Find which way up/down the list that boosts will be applied
	bool fromTop = false;
	int i = rand() % 100;
	if (i >= 50) //50/50 chance
	{
		fromTop = true;
	}

	//Apply boost bool
	float boostChance = (difficulty - GC::D_NIGHTMARE) * GC::ENEMY_BOOST_CHANCE;
	int boostCounter = 0;

	while (boostChance > GC::ZERO && boostCounter != GC::MAX_ENEMIES_SPAWNED)
	{
		if (boostChance >= (rand() % 100))
		{
			if (fromTop)
			{
				i = GC::MAX_ENEMIES_SPAWNED - boostCounter - 1;
			}
			else
			{
				i = boostCounter;
			}

			enemies[i].boosted = true;
			boostCounter += 1;
		}

		boostChance -= 100.f;
	}
}

//Creates an enemy wave based on difficulty, returns number of enemies
char CreateEnemyWave(std::vector<Enemy>& enemies, const short& difficulty)
{
	char enemiesCreated = 0;
	enemies[0].ID = 0;
	enemies[1].ID = 0;
	
	switch (difficulty)
	{
	case GC::D_TRIVIAL:
		enemiesCreated = 2;
		break;

	case GC::D_WALK_IN_THE_PARK:
		enemies[2].ID = 1;
		enemiesCreated = 3;
		break;

	case GC::D_VERY_EASY:
		enemies[2].ID = rand() % 2;
		enemiesCreated = 3;
		break;

	case GC::D_EASY:
		enemies[1].ID = 1;
		enemies[2].ID = rand() % 2;
		enemiesCreated = 3;
		break;

	case GC::D_KINDA_EASY:
		enemies[2].ID = rand() % 2;
		enemies[3].ID = rand() % 2;
		enemiesCreated = 4;
		break;

	case GC::D_NORMAL:
		enemies[2].ID = rand() % 2;
		enemies[3].ID = rand() % 2;
		enemiesCreated = 4;
		break;

	case GC::D_KINDA_HARD:
		enemies[2].ID = rand() % 2;
		enemies[3].ID = (rand() % 2) + 1;
		enemiesCreated = 4;
		break;

	case GC::D_HARD:
		enemies[2].ID = (rand() % 2) + 1;
		enemies[3].ID = (rand() % 2) + 1;
		enemiesCreated = 4;
		break;

	case GC::D_VERY_HARD:
		enemies[2].ID = (rand() % 2) + 1;
		enemies[3].ID = (rand() % 2) + 1;
		enemies[4].ID = (rand() % 2) + 1;
		enemiesCreated = 5;
		break;

	case GC::D_BRUTAL:
		enemies[2].ID = rand() % 2;
		enemies[3].ID = (rand() % 2) + 1;
		enemies[4].ID = 3;
		enemiesCreated = 5;
		break;

	case GC::D_NIGHTMARE:
		enemies[0].ID = rand() % 2;
		enemies[1].ID = rand() % 2;
		enemies[2].ID = rand() % 2;
		enemies[3].ID = (rand() % 2) + 1;
		enemies[4].ID = 3;
		enemiesCreated = 5;
		break;

	default:
		enemies[0].ID = rand() % 2;
		enemies[1].ID = rand() % 2;
		enemies[2].ID = rand() % 2;
		enemies[3].ID = (rand() % 2) + 1;
		enemies[4].ID = 3;
		enemiesCreated = 5;
		BoostWave(enemies, difficulty);
	}

	return enemiesCreated;
}

//Deactivates all enemies and stops their sound
void StopSounds(std::vector<Enemy>& enemies, Player& player)
{
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i].active = false;
		enemies[i].entity.footsteps.stop();
		enemies[i].entity.noise.stop();
	}

	player.entity.footsteps.stop();
	player.entity.noise.stop();
}

void Game::Init(sf::RenderWindow& window, Input& input)
{
	//Random
	srand(static_cast<int>(time(0))); //Sets random's seed to current time, for "true random"

	//Game data
	data.Init(window, input);

	//UI
	ui.Init(data);

	//Rooms
	roomList.resize(GC::ROOM_NUM);
	roomList[0].Init(data, 0, { 16, 16 });
	roomList[1].Init(data, 1, { 48, 16 });
	roomList[2].Init(data, 2, { 48, 48 });
	roomList[3].Init(data, 3, { 16, 48 });

	//Player
	player1.Init(data, input);
	data.playerHit = &player1.hit;

	//Enemy
	enemyList.resize(GC::MAX_ENEMIES);

	//Projectiles
	projectiles.resize(GC::MAX_PROJECTILES);
	InitProjectiles(data, projectiles);

	//Audio
	menuHover.setBuffer(data.sounds[GC::SOUND_MENU_HOVER]);
	menuSelect.setBuffer(data.sounds[GC::SOUND_MENU_SELECT]);
}

void Game::IsPlayerDead(sf::RenderWindow& window, int& state)
{
	if (data.playerDead)
	{
		//SAVE METRICS TO DATABASE
		
		//END THE GAME SESSION
		PlayMusic(data.music, GC::MUSIC_LOSE, false);
		printf("I need to do things because the player is dead!\n");
		ExitGame(window, state, true);
	}
}

void Game::ExitGame(sf::RenderWindow& window, int& state, const bool& playerDead)
{
	state = GC::STATE_MAIN_MENU;
	window.setView(window.getDefaultView());

	//Audio
	if (!playerDead)
	{
		data.music.stop();
	}

	StopSounds(enemyList, player1);
}

void Game::EnemyUpdate()
{
	for (unsigned char index = 0; index < enemyList.size(); index++)
	{
		if (enemyList[index].active)
		{
			enemyList[index].Update(data, projectiles, roomList, player1.entity);
		}
	}

	//Enemy wave spawning
	enemySpawnTimer -= data.elapsed;

	if (enemySpawnTimer < GC::ZERO)
	{
		enemySpawnTimer = GC::ENEMY_SPAWN_TIME;
		SpawnEnemies();
	}
}

void Game::EnemyRender(sf::RenderWindow& window)
{
	for (unsigned char index = 0; index < enemyList.size(); index++)
	{
		if (enemyList[index].active)
		{
			enemyList[index].entity.Render(window, data);
		}
	}
}

void Game::SpawnEnemies()
{
	//Get difficulty rating
	short difficulty = GetDifficultyRating(data.metrics.totalTime);

	//Create an enemy wave based on difficulty
	std::vector<Enemy> enemies(GC::MAX_ENEMIES_SPAWNED);
	char numEnemies = CreateEnemyWave(enemies, difficulty);

	//Spawn the wave
	bool found = false;
	unsigned char enemyListIndex = 0;
	for (char index = 0; index < numEnemies; index++)
	{
		while (!found && (enemyListIndex < enemyList.size()))
		{
			if (!enemyList[enemyListIndex].active)
			{
				found = true;
				enemyList[enemyListIndex] = enemies[index];
				Dim2Df spawnLocation = GetValidEnemySpawn(roomList, data.playerPosition, player1.entity.roomID);
				enemyList[enemyListIndex].Init(data, spawnLocation, enemyListIndex);
			}

			enemyListIndex++;
		}

		found = false;
	}
}

void Game::Update(sf::RenderWindow& window, const Input& input, int& state)
{
	//Check for end game
	IsPlayerDead(window, state);

	if (input.escapePressed)
	{
		ExitGame(window, state, false);
	}

	//Clock
	data.elapsed = clock.getElapsedTime().asSeconds();
	data.metrics.totalTime += data.elapsed;
	if (data.elapsed > GC::APPROX_ELAPSED)
	{
		data.elapsed = GC::APPROX_ELAPSED; //Clamp elapsed time, for testing purposes
	}
	clock.restart();

	//Player
	player1.Update(data, projectiles, enemyList, roomList);

	//Enemy
	EnemyUpdate();

	//Projectiles
	UpdateProjectiles(data, projectiles, enemyList, player1);
}

void Game::Render(sf::RenderWindow& window)
{
	//Map
	data.RenderMap(window, player1.entity.sprite.getPosition());

	//Rooms
	RenderRooms(data, window, roomList);

	//Enemy
	EnemyRender(window);

	//Projectiles
	RenderProjectiles(window, projectiles);

	//Player
	player1.entity.Render(window, data);

	//UI
	ui.Render(data, window, player1.entity.health, player1.coins);
}

void Game::NewGame(sf::RenderWindow& window)
{
	//View
	window.setView(data.camera);

	//Shops
	roomList[0].shopCounter = 0;
	roomList[1].shopCounter = 0;
	roomList[2].shopCounter = 0;
	roomList[3].shopCounter = 0;
	InitShops(data, roomList);

	//Player: Stats
	player1.coins = 0;
	player1.maxHealth = GC::PLAYER_HEALTH;
	player1.entity.health = GC::PLAYER_HEALTH;
	player1.speed = GC::MEDIUM_MOVEMENT_SPEED;
	player1.entity.power = 1.f;
	player1.knockbackPower = 100.f;
	player1.entity.attackSpeed = 1.f;
	//Player: Position
	player1.entity.sprite.setPosition(GetRandomSpawn(roomList, true, 0));
	player1.entity.FindCurrentRoom(roomList);
	//Player: Weapon
	player1.entity.weapon = GC::SWORD;
	player1.entity.weapon.Init(data, player1.entity.isPlayer, player1.entity.anim, player1.entity.weaponNoise);
	player1.entity.canAttack = true;

	//Enemy
	for (unsigned int i = 0; i < enemyList.size(); i++)
	{
		enemyList[i].active = false;
	}
	enemySpawnTimer = GC::ENEMY_SPAWN_TIME;

	//Game data
	data.metrics = Metrics{};
	data.playerDead = false;
	data.metrics.totalTime = 300.f;

	//Time
	data.elapsed = 0.f;
	clock.restart();
}