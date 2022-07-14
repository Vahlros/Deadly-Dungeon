#include "data.h"

void CentreTextOrigin(sf::Text& text)
{
	text.setScale({ 1.f, 1.f }); //Reset scaling, any different scale messes with the code below
	const auto localBounds = sf::Vector2f(text.getLocalBounds().left, text.getLocalBounds().top);
	const auto globalOrigin = sf::Vector2f(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
	text.setScale(GC::TEXT_SCALING);

	text.setOrigin(localBounds + globalOrigin);
}

void Metrics::InitDatabase()
{
	//check the database is setup
	bool doesExist;
	database.Init("bin/scores.db", doesExist);

	if (!doesExist)
	{
		database.ExecQuery("CREATE TABLE Main_Stats(" \
			"ID				INTEGER		NOT NULL	PRIMARY KEY		AUTOINCREMENT, "\
			"Time			FLOAT		NOT NULL, " \
			"Kills			INTEGER		NOT NULL, " \
			"Coins_Earned	INTEGER		NOT NULL)");

		database.ExecQuery("CREATE TABLE Kills(" \
			"ID				INTEGER		NOT NULL	PRIMARY KEY		AUTOINCREMENT, "\
			"Imp			INTEGER		NOT NULL, " \
			"Lesser_Demon	INTEGER		NOT NULL, " \
			"Aberrant		INTEGER		NOT NULL, " \
			"Greater_Demon	INTEGER		NOT NULL)");

		database.ExecQuery("CREATE TABLE Weapon_Kills(" \
			"ID				INTEGER		NOT NULL	PRIMARY KEY		AUTOINCREMENT, "\
			"Sword			INTEGER		NOT NULL, " \
			"Fancy_Sword	INTEGER		NOT NULL, " \
			"Spear			INTEGER		NOT NULL, " \
			"Projectiles	INTEGER		NOT NULL)");

		database.ExecQuery("CREATE TABLE Damage_Taken(" \
			"ID				INTEGER		NOT NULL	PRIMARY KEY		AUTOINCREMENT, "\
			"Imp			INTEGER		NOT NULL, " \
			"Lesser_Demon	INTEGER		NOT NULL, " \
			"Aberrant		INTEGER		NOT NULL, " \
			"Greater_Demon	INTEGER		NOT NULL)");

		database.ExecQuery("CREATE TABLE Shop(" \
			"ID					INTEGER		NOT NULL	PRIMARY KEY		AUTOINCREMENT, "\
			"Items_Purchased	INTEGER		NOT NULL, " \
			"Heals_Purchased	INTEGER		NOT NULL, " \
			"Coins_Spent		INTEGER		NOT NULL)");
	}
}

void Metrics::SaveScores()
{
	//Add new entries
	database.ExecQuery("INSERT INTO Main_Stats (Time, Kills, Coins_Earned) " \
		"VALUES (" + std::to_string(totalTime) + ", " + std::to_string(enemiesKilled) + ", " \
		+ std::to_string(coinsEarned) + ")");

	database.ExecQuery("INSERT INTO Kills (Imp, Lesser_Demon, Aberrant, Greater_Demon) " \
		"VALUES (" + std::to_string(impKills) + ", " + std::to_string(lesserDemonKills) + ", " \
		+ std::to_string(aberrantKills) + ", " + std::to_string(greaterDemonKills) + ")");

	database.ExecQuery("INSERT INTO Weapon_Kills (Sword, Fancy_Sword, Spear, Projectiles) " \
		"VALUES (" + std::to_string(swordKills) + ", " + std::to_string(fancySwordKills) + ", " \
		+ std::to_string(spearKills) + ", " + std::to_string(projectileKills) + ")");

	database.ExecQuery("INSERT INTO Damage_Taken (Imp, Lesser_Demon, Aberrant, Greater_Demon) " \
		"VALUES (" + std::to_string(impPlayerDamage) + ", " + std::to_string(lesserDemonPlayerDamage) + ", " \
		+ std::to_string(aberrantPlayerDamage) + ", " + std::to_string(greaterDemonPlayerDamage) + ")");

	database.ExecQuery("INSERT INTO Shop (Items_Purchased, Heals_Purchased, Coins_Spent) " \
		"VALUES (" + std::to_string(itemsPurchased) + ", " + std::to_string(healsPurchased) + ", " \
		+ std::to_string(coinsSpent) + ")");

	//Save
	database.SaveToDisk();
}

void Metrics::UpdatePlayerDamage(const char& enemyID, const char& damage)
{
	switch (enemyID)
	{
	case GC::ID_IMP:
		impPlayerDamage += damage;
		break;

	case GC::ID_LESSER_DEMON:
		lesserDemonPlayerDamage += damage;
		break;

	case GC::ID_ABERRANT:
		aberrantPlayerDamage += damage;
		break;

	case GC::ID_GREATER_DEMON:
		greaterDemonPlayerDamage += damage;
		break;

	default:
		printf("Enemy ID not recognised, player damage was not updated\n");
	}
}

void Metrics::UpdateKills(const char& weaponID, const char& enemyID)
{
	enemiesKilled += 1;

	switch (weaponID)
	{
	case GC::ID_SWORD:
		swordKills += 1;
		break;

	case GC::ID_FANCY_SWORD:
		fancySwordKills += 1;
		break;

	case GC::ID_SPEAR:
		spearKills += 1;
		break;

	case GC::ID_PROJECTILE:
		projectileKills += 1;
		break;

	default:
		printf("Weapon ID not recognised, weapon kill was not updated\n");
	}

	switch (enemyID)
	{
	case GC::ID_IMP:
		impKills += 1;
		coinsEarned += GC::IMP_COINS;
		break;

	case GC::ID_LESSER_DEMON:
		lesserDemonKills += 1;
		coinsEarned += GC::LESSER_DEMON_COINS;
		break;

	case GC::ID_ABERRANT:
		aberrantKills += 1;
		coinsEarned += GC::ABERRANT_COINS;
		break;

	case GC::ID_GREATER_DEMON:
		greaterDemonKills += 1;
		coinsEarned += GC::GREATER_DEMON_COINS;
		break;

	default:
		printf("Enemy ID not recognised, enemy kill was not updated\n");
	}
}

void Metrics::UpdatePurchases(const char& itemID, const char& price)
{
	coinsSpent += price;
	itemsPurchased += 1;

	switch (itemID)
	{
	case GC::WS_HEALTH:
		purchasedHealthMax = true;
		break;

	case GC::WS_SPEED:
		purchasedSpeed = true;
		break;

	case GC::WS_POWER:
		purchasedPower = true;
		break;

	case GC::WS_ATTACK_SPEED:
		purchasedAttackSpeed = true;
		break;

	case GC::WS_KNOCKBACK:
		purchasedKnockback = true;
		break;

	case GC::WS_FULL_HEAL:
		healsPurchased += 1;
		break;

	case GC::LS_FANCY_SWORD:
		break;

	case GC::LS_SPEAR:
		break;

	case GC::LS_BIG_WEAPONS:
		purchasedBigWeapons = true;
		break;

	case GC::LS_MELEE_PROJECTILE:
		purchasedMeleeBullets = true;
		meleeBulletUpgrades += 1;
		break;

	default:
		printf("Item ID not recognised, player damage was not updated\n");
	}
}

void GameData::Init(sf::RenderWindow& window, Input& inputRef)
{
	//Input
	input = &inputRef;

	//Resolution
	screenResolution.x = sf::VideoMode::getDesktopMode().width;
	screenResolution.y = sf::VideoMode::getDesktopMode().height;

	//Vectors
	textures.resize(GC::NUM_TEXTURES);
	collisionMap.resize(GC::MAP_SIZE_TILES);
	for (unsigned char index = 0; index < GC::MAP_SIZE_TILES; ++index)
	{
		collisionMap[index].resize(GC::MAP_SIZE_TILES);
	}

	//Spritesheets
	textures[GC::SPRITESHEET_TEXTURE].create(512, 512);
	textures[GC::SPRITESHEET_TEXTURE].loadFromFile("graphics/spritesheet.png");
	spritesheetImg.create(512, 512);
	spritesheetImg.loadFromFile("graphics/spritesheet.png");

	//Map & Camera Textures
	textures[GC::MAP_FLOOR_TEXTURE].create(GC::MAP_SIZE_PIXELS, GC::MAP_SIZE_PIXELS);
	textures[GC::TILE_TEXTURE].create(GC::TILE_SIZE, GC::TILE_SIZE);
	textures[GC::WALL_SIDE_TEXTURE].create(GC::WALL_SIDE_WIDTH, GC::TILE_SIZE);
	textures[GC::WALL_TOP_TEXTURE].create(GC::TILE_SIZE, GC::WALL_TOP_HEIGHT);
	textures[GC::WALL_SIDE_TOP_TEXTURE].create(GC::WALL_SIDE_WIDTH, GC::WALL_TOP_HEIGHT);
	textures[GC::LAVA_FOUNTAIN_TEXTURE].create(GC::FOUNTAIN_ANIM_LAVA_RECT.width, GC::FOUNTAIN_ANIM_LAVA_RECT.height);
	textures[GC::LAVA_FOUNTAIN_TEXTURE].loadFromImage(spritesheetImg, GC::FOUNTAIN_ANIM_LAVA_RECT);
	textures[GC::WATER_FOUNTAIN_TEXTURE].create(GC::FOUNTAIN_ANIM_WATER_RECT.width, GC::FOUNTAIN_ANIM_WATER_RECT.height);
	textures[GC::WATER_FOUNTAIN_TEXTURE].loadFromImage(spritesheetImg, GC::FOUNTAIN_ANIM_WATER_RECT);

	//Water fountain texture correction
	sf::Texture fountainTop;
	fountainTop.create(GC::FOUNTAIN_ANIM_TOP_RECT.width, GC::FOUNTAIN_ANIM_TOP_RECT.height);
	fountainTop.loadFromImage(spritesheetImg, GC::FOUNTAIN_ANIM_TOP_RECT);
	textures[GC::WATER_FOUNTAIN_TEXTURE].update(fountainTop);

	//Entity Textures
	textures[GC::KNIGHT_TEXTURE].create(GC::KNIGHT_ANIM_RECT.width, GC::KNIGHT_ANIM_RECT.height);
	textures[GC::KNIGHT_TEXTURE].loadFromImage(spritesheetImg, GC::KNIGHT_ANIM_RECT);
	textures[GC::IMP_TEXTURE].create(GC::IMP_ANIM_RECT.width, GC::IMP_ANIM_RECT.height);
	textures[GC::IMP_TEXTURE].loadFromImage(spritesheetImg, GC::IMP_ANIM_RECT);
	textures[GC::L_DEMON_TEXTURE].create(GC::L_DEMON_ANIM_RECT.width, GC::L_DEMON_ANIM_RECT.height);
	textures[GC::L_DEMON_TEXTURE].loadFromImage(spritesheetImg, GC::L_DEMON_ANIM_RECT);
	textures[GC::ABERRANT_TEXTURE].create(GC::ABERRANT_ANIM_RECT.width, GC::ABERRANT_ANIM_RECT.height);
	textures[GC::ABERRANT_TEXTURE].loadFromImage(spritesheetImg, GC::ABERRANT_ANIM_RECT);
	textures[GC::G_DEMON_TEXTURE].create(GC::G_DEMON_ANIM_RECT.width, GC::G_DEMON_ANIM_RECT.height);
	textures[GC::G_DEMON_TEXTURE].loadFromImage(spritesheetImg, GC::G_DEMON_ANIM_RECT);

	//Projectile Textures
	textures[GC::FIRE_SKULL_TEXTURE].create(GC::FIRE_SKULL_ANIM_RECT.width, GC::FIRE_SKULL_ANIM_RECT.height);
	textures[GC::FIRE_SKULL_TEXTURE].loadFromImage(spritesheetImg, GC::FIRE_SKULL_ANIM_RECT);
	textures[GC::FIRE_BALL_TEXTURE].create(GC::FIRE_BALL_ANIM_RECT.width, GC::FIRE_BALL_ANIM_RECT.height);
	textures[GC::FIRE_BALL_TEXTURE].loadFromImage(spritesheetImg, GC::FIRE_BALL_ANIM_RECT);
	textures[GC::FROST_BALL_TEXTURE].create(GC::FROST_BALL_ANIM_RECT.width, GC::FROST_BALL_ANIM_RECT.height);
	textures[GC::FROST_BALL_TEXTURE].loadFromImage(spritesheetImg, GC::FROST_BALL_ANIM_RECT);

	//UI Textures
	textures[GC::COIN_TEXTURE].create(GC::COINS_ANIM_RECT.width, GC::COINS_ANIM_RECT.height);
	textures[GC::COIN_TEXTURE].loadFromImage(spritesheetImg, GC::COINS_ANIM_RECT);

	//Scaling
	if ((screenResolution.x == 3840) && (screenResolution.y == 2160))
	{
		scaling = GC::SCALE_2160;
	}
	else if ((screenResolution.x == 2560) && (screenResolution.y == 1440))
	{
		scaling = GC::SCALE_1440;
	}
	else if ((screenResolution.x == 1920) && (screenResolution.y == 1080))
	{
		scaling = GC::SCALE_1080;
	}

	//Camera
	cameraDimensions.x = static_cast<float>(screenResolution.x) / scaling;
	cameraDimensions.y = static_cast<float>(screenResolution.y) / scaling;

	camera.setSize(static_cast<float>(cameraDimensions.x), static_cast<float>(cameraDimensions.y));

	//Sprites
	mapSprite.setTexture(textures[GC::MAP_FLOOR_TEXTURE], true);

	//Font
	font.loadFromFile("graphics/SHOWG.ttf");

	//Audio
	InitAudio(sounds);


	metrics.InitDatabase();
}

void GameData::RenderMap(sf::RenderWindow& window, const Dim2Df position)
{
	playerPosition = position;
	camera.setCenter(playerPosition);
	window.setView(camera);
	window.draw(mapSprite);
}