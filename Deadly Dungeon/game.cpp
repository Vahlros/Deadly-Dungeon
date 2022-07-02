#include "game.h"
#include "maths.h"

//Initializes game session
void Game::Init(sf::RenderWindow& window)
{
	//Game data
	data.Init(window);

	//Rooms
	Room startRoom;
	startRoom.Init(data, 0, { 32, 32 });
	Room enemyRoom;
	enemyRoom.Init(data, 1, { 32, 56 });
	Room itemRoom;
	itemRoom.Init(data, 2, { 32, 8 });

	roomList.resize(3);
	roomList[0] = startRoom;
	roomList[1] = enemyRoom;
	roomList[2] = itemRoom;

	//Player
	player1.Init(data);
	data.playerHit = &player1.hit;

	//Enemy
	Enemy enemy1;
	enemy1.ID = 1;
	enemy1.Init(data, { 700.f, 700.f });

	enemyList.resize(10);
	enemyList[0] = enemy1;

	//Projectiles
	projectiles.resize(GC::MAX_PROJECTILES);
	InitProjectiles(data, projectiles);

	//Random
	srand((int)time(0)); //Sets random's seed to current time, for "true random"
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

	//Render animated tiles
	roomList[0].UpdateAnimatedTiles(data, window);
	roomList[1].UpdateAnimatedTiles(data, window);
	roomList[2].UpdateAnimatedTiles(data, window);

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
		//Calculate distance to enemy
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