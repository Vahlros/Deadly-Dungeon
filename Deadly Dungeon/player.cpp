#include "player.h"
#include "maths.h"

//Initializes player
void Player::Init(GameData& game)
{
	//Stats
	speed = GC::MEDIUM_MOVEMENT_SPEED;

	//Entity stats
	entity.isPlayer = true;
	entity.health = 10;
	entity.collisionRect = GC::KNIGHT_BODY_RECT;
	entity.anim.Init(&GC::PLAYER_ANIM_IDLE);

	//SFML
	entity.texture = &game.textures[GC::KNIGHT_TEXTURE];
	entity.sprite.setTexture(*entity.texture);
	entity.sprite.setTextureRect({ 0, 0, GC::KNIGHT_DIMENSIONS.x, GC::KNIGHT_DIMENSIONS.y });
	entity.sprite.setOrigin(Dim2Df(GC::KNIGHT_BODY_CENTRE));
	entity.sprite.setPosition(GC::START_POSITION);

	//Weapon
	entity.weapon = GC::SWORD;
	entity.weapon.Init(game, entity.isPlayer);
}

//Get inputs and react
void Player::InputHandling(sf::RenderWindow& window, const GameData& game)
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		//Exit application
		if ((event.type == sf::Event::Closed) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		//Play inputs
		if (game.input == GC::GAMEPAD)
		{
			GamepadControls(event);
		}
		else
		{
			KeyboardControls(event, game);
		}
	}
}

//Controls for keyboard
void Player::KeyboardControls(const sf::Event& event, const GameData& game)
{
	//Movement
	KeyboardMovement(event);

	//Entity facing
	GetMouseAngle(game);

	//Attacks
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (entity.canAttack && !entity.weapon.attacking)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				entity.InitAttack(game, GC::FIRST_ATTACK);
				entity.weapon.attack0.attackRelease = false;
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				entity.InitAttack(game, GC::SECOND_ATTACK);
				entity.weapon.attack1.attackRelease = false;
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			entity.weapon.attack0.attackRelease = true;
		}
		if (event.mouseButton.button == sf::Mouse::Right)
		{
			entity.weapon.attack1.attackRelease = true;
		}
	}

	//Attack swapping for testing
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		entity.weapon = GC::SWORD;
		entity.weapon.Init(game, entity.isPlayer);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		entity.weapon = GC::SPEAR;
		entity.weapon.Init(game, entity.isPlayer);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		//entity.weapon.attack0 = GC::SWORD_OF_DOOM_ATTACK;
		entity.weapon = GC::RUSTED_SWORD;
		entity.weapon.Init(game, entity.isPlayer);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		//entity.weapon.attack0 = GC::NORMAL_STRAIGHT_THROW_ATTACK;
		//entity.weapon.attack1 = GC::NORMAL_SPINNING_THROW_ATTACK;
		entity.weapon = GC::FANCY_SWORD;
		entity.weapon.Init(game, entity.isPlayer);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		//Do nothing
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		entity.weapon = GC::IMP_WEAPON;
		entity.weapon.Init(game, entity.isPlayer);
	}
}

//Find movement vector based on isKeyPressed booleans
void Player::KeyboardMovement(const sf::Event& event)
{
	//Movement booleans
	bool movingLeft = false; //Player is moving left
	bool movingRight = false; //Player is moving right
	bool movingUp = false; //Player is moving up
	bool movingDown = false; //Player is moving down

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		movingLeft = true;
	}
	else
	{
		movingLeft = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movingRight = true;
	}
	else
	{
		movingRight = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		movingUp = true;
	}
	else
	{
		movingUp = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movingDown = true;
	}
	else
	{
		movingDown = false;
	}

	//Opposite directions cancel each other out
	if (movingLeft && movingRight)
	{
		movingLeft = false;
		movingRight = false;
	}
	if (movingUp && movingDown)
	{
		movingUp = false;
		movingDown = false;
	}

	//Update movement bool and animation
	if (movingUp || movingDown || movingLeft || movingRight)
	{
		if (!entity.moving)
		{
			entity.moving = true;
			entity.anim.Init(&GC::PLAYER_ANIM_MOVE);
		}
	}
	else
	{
		if (entity.moving)
		{
			entity.moving = false;
			entity.anim.Init(&GC::PLAYER_ANIM_IDLE);
		}
	}

	//entity.moving = true;
	if (entity.moving)
	{
		//Find movement vector
		DirectionalAngle dirAngle;

		if (movingRight && movingUp) //North directional angle
		{
			dirAngle.angle = GC::RADS_45DEGREES;
			dirAngle.direction = GC::NORTH;
			entity.movementVector = CalculateVectorOfMagnitude(dirAngle, speed);
		}
		else if (movingRight && movingDown) //East directional angle
		{
			dirAngle.angle = GC::RADS_45DEGREES;
			dirAngle.direction = GC::EAST;
			entity.movementVector = CalculateVectorOfMagnitude(dirAngle, speed);
		}
		else if (movingLeft && movingDown) //South directional angle
		{
			dirAngle.angle = GC::RADS_45DEGREES;
			dirAngle.direction = GC::SOUTH;
			entity.movementVector = CalculateVectorOfMagnitude(dirAngle, speed);
		}
		else if (movingLeft && movingUp) //West directional angle
		{
			dirAngle.angle = GC::RADS_45DEGREES;
			dirAngle.direction = GC::WEST;
			entity.movementVector = CalculateVectorOfMagnitude(dirAngle, speed);
		}
		else if (movingUp) //North
		{
			entity.movementVector = { 0.f, -speed };
		}
		else if (movingRight) //East
		{
			entity.movementVector = { speed, 0.f };
		}
		else if (movingDown) //South
		{
			entity.movementVector = { 0.f, speed };
		}
		else //West
		{
			entity.movementVector = { -speed, 0.f };
		}
	}
}

//Find the entity's facing angle based on mouse position
void Player::GetMouseAngle(const GameData& game)
{
	//Find origin and target points
	Dim2Df centre = { (float)(game.screenResolution.x / 2), (float)(game.screenResolution.y / 2) };
	Dim2Df mousePosition = { (float)mouse.getPosition().x, (float)mouse.getPosition().y };

	//Get vector between points, then calculate directional angle using vector
	entity.facing = CalculateDirectionalAngleFromVector(CalculateVectorBetweenPoints(centre, mousePosition));
}

//Controls for gamepad
void Player::GamepadControls(const sf::Event& event)
{

}

//Checks attack collision against enemies
void Player::CheckAttackCollision(std::vector<Enemy>& enemies)
{
	if (entity.weapon.attacking && entity.weapon.CheckIfMotionCanDamage())
	{
		for (char index = 0; index < GC::MAX_ENEMIES; index++)
		{
			if (enemies[index].active && !enemies[index].entity.invulnerable)
			{
				//Calculate distance to enemy
				Dim2Df position = enemies[index].entity.sprite.getPosition();
				float distanceToEnemy = CalculateMagnitudeOfVector(entity.weapon.sprite.getPosition() - position);

				//If in range, check collision
				if (distanceToEnemy <= GC::CHECK_ATTACK_COLLISION_RANGE)
				{
					if (entity.weapon.sprite.getGlobalBounds().intersects(enemies[index].entity.sprite.getGlobalBounds()))
					{
						//Calculate damage
						float damage = entity.power * GC::DEFAULT_DAMAGE;

						if (entity.weapon.attack1.active) //Heavy attack
						{
							damage *= heavyAttackMultiplier;
						}

						unsigned char actualDamage = (unsigned char)round(damage);
						enemies[index].entity.TakeDamage(actualDamage, entity.facing, knockbackPower);
					}
				}
			}
		}
	}
}

//Updates player state
void Player::Update(sf::RenderWindow& window, const GameData& game, std::vector<Projectile> projectiles, std::vector<Enemy>& enemies)
{
	//Invulnerability
	UpdateInvulnerability(game);

	//Input
	InputHandling(window, game);

	//Movement
	if (entity.moving && !dodging)
	{
		entity.Move(game);
	}

	//Weapon
	entity.UpdateWeapon(game, projectiles);
	CheckAttackCollision(enemies);
}

//Handles invulnerability
void Player::UpdateInvulnerability(const GameData& game)
{
	//Invulnerability
	if (hit)
	{
		if (entity.health <= 0)
		{
			printf("my balls tingle when your mum touches them\n");
		}

		entity.invulnerable = true;
		entity.invulnerabilityTimer = GC::PLAYER_HIT_INVULNERABILITY;
		entity.sprite.setColor(GC::PLAYER_HIT_COLOUR);
		entity.weapon.sprite.setColor(GC::PLAYER_HIT_COLOUR);
		hit = false;
	}
	else if (entity.invulnerable)
	{
		if (entity.invulnerabilityTimer < 0.f)
		{
			entity.invulnerable = false;
			entity.sprite.setColor(sf::Color::White);
			entity.weapon.sprite.setColor(sf::Color::White);
		}
		else
		{
			entity.invulnerabilityTimer -= game.elapsed;
		}
	}
}