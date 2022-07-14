#pragma once
#include <vector>
#include "Utils.h"
#include "MyDB.h"
#include "input.h"
#include "audio.h"

//Came from help from the SFML discord server, centres text origin
void CentreTextOrigin(sf::Text& text);

//Angle from clockwise NESW axis, calculated in radians
//Notes: Can be converted to degrees for SFML, be wary of conversion usage
struct DirectionalAngle
{
	float angle = 0.f; //Angle in radians
	char direction = 0; //Direction of the angle, default is North
};

//Tile used for tilemap
struct Tile
{
	char ID = 0; //Could be removed? Or kept for readability?
	char name[32] = "";
	sf::IntRect rect{};
};

//Data used for animations
struct AnimationData
{
	char startFrame = 0; //Frame that the animation starts from
	char endFrame = 0; //Frame that the animation ends on
	float frameChange = 0.f; //How frequent the frames will change, in seconds
};

//Data collected for scores
struct Metrics
{
	//Database
	MyDB database;

	//Counters: Game
	unsigned short enemiesKilled = 0; //How many enemies the player has killed
	unsigned short coinsEarned = 0; //How many coins the player has picked up
	float totalTime = 0.f; //How long the player has survived
	//Counters: Shop
	unsigned char itemsPurchased = 0; //How many items the player has purchased
	unsigned short healsPurchased = 0; //How many heals the player has purchased
	unsigned short coinsSpent = 0; //How many coins the player has spent
	unsigned char meleeBulletUpgrades = 0; //How many projectiles are fired upon weapon swing
	//Counters: Weapon
	unsigned short swordKills = 0; //How many enemies killed with the sword
	unsigned short fancySwordKills = 0; //How many enemies killed with the fancy sword
	unsigned short spearKills = 0; //How many enemies killed with the spear
	unsigned short projectileKills = 0; //How many enemies killed with projectiles
	//Counters: Enemy
	unsigned short impKills = 0; //How many imps were killed
	unsigned short lesserDemonKills = 0; //How many imps were killed
	unsigned short aberrantKills = 0; //How many imps were killed
	unsigned short greaterDemonKills = 0; //How many imps were killed
	//Counters: Damage Taken
	unsigned short impPlayerDamage = 0; //How much damage imps dealt to the player
	unsigned short lesserDemonPlayerDamage = 0; //How much damage lesser demons dealt to the player
	unsigned short aberrantPlayerDamage = 0; //How much damage aberrants dealt to the player
	unsigned short greaterDemonPlayerDamage = 0; //How much damage greater demons dealt to the player

	//Bools
	bool purchasedHealthMax = false; //If the player has the health boost active
	bool purchasedSpeed = false; //If the player has the speed boost active
	bool purchasedPower = false; //If the player has the power boost active
	bool purchasedAttackSpeed = false; //If the player has the attacks speed boost active
	bool purchasedKnockback = false; //If the player has the knockback boost active
	bool purchasedBigWeapons = false; //If the player has the big weapons boost active
	bool purchasedMeleeBullets = false; //If the player has the melee projectiles boost active

	//Initializes the database, creating one if it does not exist
	void InitDatabase();

	//Enters all metrics from that game into the database, called on death or win
	void SaveScores();

	//Update player's damage taken
	void UpdatePlayerDamage(const char& enemyID, const char& damage);

	//Update player's kills
	void UpdateKills(const char& weaponID, const char& enemyID);

	//Update player's purchases
	void UpdatePurchases(const char& itemID, const char& price);
};


//Data for current game session
struct GameData
{
	//Gameplay metrics
	Metrics metrics{}; //Player metrics for this game

	//Window and camera
	Dim2Di screenResolution = { 0, 0 }; //Resolution of the player's screen
	float scaling = 0.f; //View scaling, varies based on the screen size
	Dim2Df cameraDimensions = { 0, 0 }; //Global position and area of the camera, in pixels
	sf::View camera{}; //View for the window, zoomed in so that the map and sprites can be drawn in pixels

	//Operational bools
	bool playerDead = false; //If the player has died

	//Timers
	float elapsed = 0.f; //Time elapsed since last frame

	//Positioning
	std::vector<std::vector<int>> collisionMap; //Vector of map collision data
	Dim2Df playerPosition{};

	//Input
	Input* input;

	//Audio
	sf::Music music; //Background music of current state
	std::vector<sf::SoundBuffer> sounds; //Vector of all game sounds

	//SFML
	std::vector<sf::Texture> textures; //Vector of all textures
	sf::Image spritesheetImg{}; //Spritesheet in Image format, for texture.loadFromImage()
	sf::Sprite mapSprite{}; //Sprite for the environment
	sf::Font font{}; //Game font

	//Pointers
	bool* playerHit = nullptr; //If the player has just been hit

	//Initializes game session
	void Init(sf::RenderWindow& window, Input& inputRef);

	//Renders the map onto the camera, based on player position
	void RenderMap(sf::RenderWindow& window, const Dim2Df position);
};

//Game constants
namespace GC
{
	//Enums
	enum GAME_STATE { STATE_MAIN_MENU, STATE_SCORES, STATE_TUTORIAL, STATE_PLAYING, STATE_WIN, STATE_LOSE }; //Current game state
	enum DIRECTIONS { NORTH, EAST, SOUTH, WEST }; //NESW directions
	enum TEXTURE_LIST { //Texture IDs
		SPRITESHEET_TEXTURE, MAP_FLOOR_TEXTURE, TILE_TEXTURE, WALL_SIDE_TEXTURE, WALL_TOP_TEXTURE, WALL_SIDE_TOP_TEXTURE, WATER_FOUNTAIN_TEXTURE, LAVA_FOUNTAIN_TEXTURE,
		KNIGHT_TEXTURE, IMP_TEXTURE, L_DEMON_TEXTURE, ABERRANT_TEXTURE, G_DEMON_TEXTURE, FIRE_SKULL_TEXTURE, FIRE_BALL_TEXTURE, FROST_BALL_TEXTURE, COIN_TEXTURE
	};
	enum PLAYER_INPUT { KEYBOARD, GAMEPAD }; //Player input states
	enum ROOM_TYPES { R32X32, R16X16, R32X16, R16X32 }; //Room types
	enum ATTACK_INPUTS { FIRST_ATTACK, SECOND_ATTACK }; //Attack inputs
	enum COLLISION_TYPES { //Collision IDs
		C_FREE_MOVEMENT, C_WALL, C_WALL_TOP, C_WALL_SIDE_LEFT, C_WALL_SIDE_RIGHT, C_WALL_TOP_BOTTOM_LEFT, C_WALL_TOP_BOTTOM_RIGHT, C_FOUNTAIN_TOP, C_FOUNTAIN_BASIN,
		C_COLUMN_TOP, C_COLUMN_BASE, C_CORNER_BOTTOM_LEFT, C_CORNER_BOTTOM_RIGHT, C_ENTITY_MOVEMENT, C_ENTITY_BODY_ATTACK, C_PROJECTILE
	};
	enum ANIMATION { IDLE = 0, MOVE = 4, DODGE = 8 }; //Animation frames
	enum ENEMY_ID { ID_IMP, ID_LESSER_DEMON, ID_ABERRANT, ID_GREATER_DEMON }; //Enemy IDs
	enum WEAPON_ID { ID_SWORD, ID_FANCY_SWORD, ID_SPEAR, ID_IMP_WEAPON, ID_LESSER_DEMON_WEAPON, ID_RUSTED_SWORD, ID_GREATER_DEMON_WEAPON, ID_PROJECTILE }; //Weapon IDs
	enum SHOP_ITEMS { WS_HEALTH, WS_SPEED, WS_POWER, WS_ATTACK_SPEED, WS_KNOCKBACK, WS_FULL_HEAL, //Shop items
		LS_FANCY_SWORD, LS_SPEAR, LS_BIG_WEAPONS, LS_MELEE_PROJECTILE };

	//Maths: General
	const float ZERO = 0.f;
	const float ONE = 1.f;

	//Screen: General
	const Dim2Di CURSOR_DIMENSIONS = { 16, 16 };
	const float SCALE_1080 = 5.f; //View zoom at 1080p
	const float SCALE_1440 = 6.4f; //View zoom at 1440p
	const float SCALE_2160 = 10.f; //View zoom at 2160p
	const unsigned char FRAMERATE = 60; //Game refresh rate
	const float APPROX_ELAPSED = 1.f / (float)GC::FRAMERATE; //Approximate elapsed time

	//Textures: General
	const char NUM_TEXTURES = 17; //Number of textures used in the game

	//Tile: General
	const char TILE_SIZE = 16; //Tile size, in pixels
	const unsigned char TILE_NUM = 67; //Total number of tiles
	//Tile: Tile dimensions
	const char WALL_SIDE_WIDTH = 5; //Width of a wall side, in pixels
	const char WALL_TOP_HEIGHT = 4; //Width of a wall top, in pixels
	const char FOUNTAIN_BASIN_HEIGHT = 9;
	const char FOUNTAIN_TOP_HEIGHT = 7;
	const char COLUMN_BASE_HEIGHT = 5;
	const char COLUMN_TOP_HEIGHT = 12;
	//Tile: Animated tiles
	const AnimationData FOUNTAIN_ANIM_LAVA = { 0, 2, 0.3f }; //Animation data for lava fountain
	const sf::IntRect FOUNTAIN_ANIM_LAVA_RECT = { 64, 9, 48, 27 }; //Where the animation is on the spritesheet
	const AnimationData FOUNTAIN_ANIM_WATER = { 0, 2, 0.3f }; //Animation data for water fountain
	const sf::IntRect FOUNTAIN_ANIM_WATER_RECT = { 64, 41, 48, 27 }; //Where the animation is on the spritesheet, minus the fountain top
	const sf::IntRect FOUNTAIN_ANIM_TOP_RECT = { 64, 9, 48, 7 }; //Fountain top rect, for drawing onto the water fountain's texture
	//Tile: Tile list
	const std::vector<Tile> TILE_LIST = {
		{0, "Darkness", {95, 495, 16, 16} },
		{1, "Wall Left", {16, 16, 16, 16} },
		{2, "Wall Mid", {32, 16, 16, 16} },
		{3, "Wall Right", {48, 16, 16, 16} },
		{4, "Wall Hole 1", {48, 32, 16, 16} },
		{5, "Wall Hole 2", {48, 48, 16, 16} },
		{6, "Wall Banner Blue", {32, 32, 16, 16} },
		{7, "Wall Banner Green", {16, 48, 16, 16} },
		{8, "Wall Banner Yellow", {32, 48, 16, 16} },
		{9, "Wall Banner Red", {16, 32, 16, 16} },
		{10, "Wall Corner Left", {32, 128, 16, 16} },
		{11, "Wall Corner Right", {48, 128, 16, 16} },
		{12, "Wall Corner Front Left", {32, 160, 16, 16} },
		{13, "Wall Corner Front Right", {48, 160, 16, 16} },
		{14, "Wall In Corner Mid Left", {80, 144, 16, 16} },
		{15, "Wall In Corner Mid Right", {64, 144, 16, 16} },
		{16, "Wall Column Mid", {96, 96, 16, 16} },
		{17, "Column Mid", {80, 96, 16, 16} },
		{18, "Lava Fountain Mid", {64, 16, 16, 16} }, //3 frame animation
		{19, "Water Fountain Mid", {64, 48, 16, 16} }, //3 frame animation
		{20, "Wall Goo", {64, 80, 16, 16} },
		{21, "Column Top", {80, 80, 16, 16} },
		{22, "Wall Column Top", {96, 80, 16, 16} },
		{23, "Fountain Top", {64, 0, 16, 16} },
		{24, "Wall Column Base", {96, 112, 16, 16} },
		{25, "Column Base", {80, 112, 16, 16}},
		{26, "Lava Fountain Basin", {64, 32, 16, 16} }, //3 frame animation
		{27, "Water Fountain Basin", {64, 64, 16, 16} }, //3 frame animation
		{28, "Wall Goo Floor", {64, 96, 16, 16} },
		{29, "Edge", {96, 128, 16, 16} },
		{30, "Wall Column Top w/ Floor", {0, 80, 16, 16} },
		{31, "Fountain Top w/ Floor", {0, 96, 16, 16} },
		{32, "Floor 1", {16, 64, 16, 16} },
		{33, "Floor 2", {32, 64, 16, 16} },
		{34, "Floor 3", {48, 64, 16, 16} },
		{35, "Floor 4", {16, 80, 16, 16} },
		{36, "Floor 5", {32, 80, 16, 16} },
		{37, "Floor 6", {48, 80, 16, 16} },
		{38, "Floor 7", {16, 96, 16, 16} },
		{39, "Floor 8", {32, 96, 16, 16} },
		{40, "Enemy Spawner", {0, 64, 16, 16} },
		{41, "Floor Spikes", {16, 176, 16, 16} }, //4 frame animation
		{42, "Floor Ladder", {48, 96, 16, 16} },
		{43, "Hole", {96, 144, 16, 16} },
		{44, "Stairs", {80, 176, 16, 16} },
		{45, "Wall Side Top Right", {11, 124, 5, 4} },
		{46, "Wall Side Top Left", {16, 124, 5, 4} },
		{47, "Wall Side Mid Right", {0, 128, 16, 16} },
		{48, "Wall Side Front Right", {0, 144, 16, 16} },
		{49, "Wall Side Mid Left", {16, 128, 16, 16} },
		{50, "Wall Side Front Left", {16, 144, 16, 16} },
		{51, "Wall Top Left", {16, 0, 16, 16} },
		{52, "Wall Top Mid", {32, 0, 16, 16} },
		{53, "Wall Top Right", {48, 0, 16, 16} },
		{54, "Wall Corner Top Left", {32, 112, 16, 16} },
		{55, "Wall Corner Top Right", {48, 112, 16, 16} },
		{56, "Wall In Corner T Top Left", {80, 160, 16, 16} },
		{57, "Wall In Corner T Top Right", {64, 160, 16, 16} },
		{58, "Wall Corner Bottom Left", {32, 144, 16, 16} },
		{59, "Wall In Corner L Top Left", {80, 128, 16, 16} },
		{60, "Wall Corner Bottom Right", {48, 144, 16, 16} },
		{61, "Wall In Corner L Top Right", {64, 128, 16, 16}},
		{62, "Water Shop", {0, 32, 16, 16}},
		{63, "Lava Shop", {0, 48, 16, 16}},
		{64, "Door Up", {16, 208, 64, 32}},
		{65, "Door Down", {16, 240, 64, 32}},
		{66, "Door Open", {80, 208, 32, 32}}
	};
	
	//Map: General
	const unsigned int MAP_SIZE_TILES = 128; //Maximum size of the map, in tiles
	const short MAP_SIZE_PIXELS = MAP_SIZE_TILES * TILE_SIZE; //Maximum size of map, in pixels

	//Player: General
	const float PLAYER_HIT_INVULNERABILITY = 2.0f; //How long the player is invulnerable after being hit
	//Player: Health
	const int PLAYER_HEALTH = 10; //Player starting health
	const int BOOSTED_HEALTH = 16; //Upgraded player health
	//Player: Animation
	const unsigned int PLAYER_ANIM_FRAMES = 9; //Number of frames
	const AnimationData PLAYER_ANIM_IDLE = { IDLE, MOVE - 1, 0.12f }; //Idle animation data for the player
	const AnimationData PLAYER_ANIM_MOVE = { MOVE, DODGE - 1, 0.12f }; //Idle animation data for the player
	//Player: Knight
	const Dim2Di KNIGHT_DIMENSIONS = { 16, 22 }; //Dimensions of the player texture
	const sf::IntRect KNIGHT_ANIM_RECT = { 128, 106, 144, KNIGHT_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect KNIGHT_BODY_RECT = { 0, 6, 16, 16 }; //Where the character's body is on the un-scaled sprite
	const Dim2Df KNIGHT_BODY_CENTRE = { 8.f, 14.f }; //Where the centre of the character's body is on the un-scaled sprite
	
	//Enemy: General
	const float ATTACK_COOLDOWN = 1.f; //Cooldown timer between attacks
	const float SPAWN_ATTACK_COOLDOWN = 0.5f; //Cooldown timer before an enemy can attack, after spawning
	//Enemy: Coins
	const int IMP_COINS = 3;
	const int LESSER_DEMON_COINS = 4;
	const int ABERRANT_COINS = 6;
	const int GREATER_DEMON_COINS = 8;
	//Enemy: Health
	const unsigned int IMP_HEALTH = 3;
	const unsigned int L_DEMON_HEALTH = 4;
	const unsigned int ABERRANT_HEALTH = 6;
	const unsigned int G_DEMON_HEALTH = 10;
	//Enemy: Animation
	const unsigned int ENEMY_ANIM_FRAMES = 8; //Number of frames
	const AnimationData ENEMY_ANIM_IDLE = { IDLE, MOVE - 1, 0.12f }; //Idle animation data for enemies
	const AnimationData ENEMY_ANIM_MOVE = { MOVE, DODGE - 1, 0.12f }; //Idle animation data for enemies
	const AnimationData ENEMY_ANIM_BITE = { IDLE, MOVE - 1, 0.2f }; //Bite animation for demons
	//Enemy: Imp Sprite
	const Dim2Di IMP_DIMENSIONS = { 16, 14 }; //Dimensions of the imp texture
	const sf::IntRect IMP_ANIM_RECT = { 369, 50, 128, IMP_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect IMP_BODY_RECT = { 4, 0, 10, IMP_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Df IMP_BODY_CENTRE = { 8.f, 9.f }; //Where the centre of the character's body is on the un-scaled sprite
	//Enemy: Lesser Demon Sprite
	const Dim2Di L_DEMON_DIMENSIONS = { 16, 23 }; //Dimensions of the lesser demon texture
	const sf::IntRect L_DEMON_ANIM_RECT = { 369, 329, 128, L_DEMON_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect L_DEMON_BODY_RECT = { 2, 0, 11, L_DEMON_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Df L_DEMON_BODY_CENTRE = { 8.f, 14.f }; //Where the centre of the character's body is on the un-scaled sprite
	//Enemy: Aberrant Sprite
	const Dim2Di ABERRANT_DIMENSIONS = { 16, 18 }; //Dimensions of the lesser demon texture
	const sf::IntRect ABERRANT_ANIM_RECT = { 368, 302, 128, ABERRANT_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect ABERRANT_BODY_RECT = { 3, 0, 10, ABERRANT_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Df ABERRANT_BODY_CENTRE = { 8.f, 12.f }; //Where the centre of the character's body is on the un-scaled sprite
	//Enemy: Greater Demon Sprite
	const Dim2Di G_DEMON_DIMENSIONS = { 32, 34 }; //Dimensions of the lesser demon texture
	const sf::IntRect G_DEMON_ANIM_RECT = { 16, 366, 256, G_DEMON_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect G_DEMON_BODY_RECT = { 5, 2, 10, G_DEMON_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Df G_DEMON_BODY_CENTRE = { 17.f, 20.f }; //Where the centre of the character's body is on the un-scaled sprite

	//Attack: General
	const unsigned int MAX_MOTIONS = 2; //Maximum number of motions per attack
	const int DEFAULT_DAMAGE = 1; //Default damage value

	//Weapon: General
	const unsigned int MAX_ATTACKS = 2; //Maximum number of attacks per weapon
	const float WEAPON_HOVER = 0.5f; //Hover distance from centre, in tiles
	const float WEAPON_HOVER_ROTATION = 30.f; //Hover rotation, in degrees
	//Weapon: Fancy Sword
	const sf::IntRect F_SWORD_RECT = { 307, 86, 10, 25 }; //Where the rusted sword is on the spritesheet
	const Dim2Df F_SWORD_ORIGIN = { 5.f, 23.f }; //The point at which the weapon is held
	//Weapon: Spear
	const sf::IntRect SPEAR_RECT = { 293, 177, 6, 30 }; //Where the sword is on the spritesheet
	const Dim2Df SPEAR_ORIGIN = { 3.f, 22.f }; //The point at which the weapon is held

	//Projectiles: General
	const short MAX_PROJECTILES = 256;
	//Projectiles: Fire Skull
	const unsigned int FIRE_SKULL_FRAMES = 6; //Number of frames
	const AnimationData FIRE_SKULL_ANIM = { 0, FIRE_SKULL_FRAMES - 1, 0.09f }; //Idle animation data for the player
	const Dim2Di FIRE_SKULL_DIMENSIONS = { 6, 6 }; //Dimensions of the player texture
	const sf::IntRect FIRE_SKULL_ANIM_RECT = { 293, 341, 36, FIRE_SKULL_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect FIRE_SKULL_BODY_RECT = { 0, 0, 6, 6 }; //Where the character's body is on the un-scaled sprite
	const Dim2Df FIRE_SKULL_BODY_CENTRE = { 2.f, 2.f }; //Where the centre of the character's body is on the un-scaled sprite
	//Projectiles: Fire Ball
	const unsigned int FIRE_BALL_FRAMES = 6; //Number of frames
	const AnimationData FIRE_BALL_ANIM = { 0, FIRE_BALL_FRAMES - 1, 0.09f }; //Idle animation data for the player
	const Dim2Di FIRE_BALL_DIMENSIONS = { 6, 6 }; //Dimensions of the player texture
	const sf::IntRect FIRE_BALL_ANIM_RECT = { 293, 349, 36, FIRE_BALL_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect FIRE_BALL_BODY_RECT = { 0, 0, 6, 6 }; //Where the character's body is on the un-scaled sprite
	const Dim2Df FIRE_BALL_BODY_CENTRE = { 2.f, 2.f }; //Where the centre of the character's body is on the un-scaled sprite
	//Projectiles: Frost Ball
	const unsigned int FROST_BALL_FRAMES = 6; //Number of frames
	const AnimationData FROST_BALL_ANIM = { 0, FROST_BALL_FRAMES - 1, 0.09f }; //Idle animation data for the player
	const Dim2Di FROST_BALL_DIMENSIONS = { 6, 6 }; //Dimensions of the player texture
	const sf::IntRect FROST_BALL_ANIM_RECT = { 293, 356, 36, FROST_BALL_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect FROST_BALL_BODY_RECT = { 0, 0, 6, 6 }; //Where the character's body is on the un-scaled sprite
	const Dim2Df FROST_BALL_BODY_CENTRE = { 2.f, 2.f }; //Where the centre of the character's body is on the un-scaled sprite

	//Collision
	const int CHECK_ATTACK_COLLISION_RANGE = TILE_SIZE * 4; //The distance between two origins in which collisions will be checked

	//Knockback
	const float KNOCKBACK_TIMER = 0.25f; //How long an enemy is knocked back for
	const float ENEMY_INVULNERABILITY = 0.25f; //How long an entity is invulnerable for in knockback

	//Colours
	const sf::Color PLAYER_COLOUR = sf::Color(51, 102, 255); //Softer blue than default
	const sf::Color FONT_COLOUR = sf::Color(211, 191, 169); //Cream-ish beige-ish
	const sf::Color ENEMY_BOOST_COLOUR = sf::Color( 42, 0, 0 ); //Evil red

	//Shop: Text
	const int TEXT_CHARACTER_SIZE = 40;
	const Dim2Df TEXT_SCALING = { 0.3f, 0.3f };

	//UI: Coins
	const Dim2Di COINS_DIMENSIONS = { 8, 8 };
	const sf::IntRect COINS_ANIM_RECT = { 288, 272, 32, 8 };
	const unsigned int COINS_FRAMES = 4;
	const AnimationData COINS_ANIM_DATA = { 0, COINS_FRAMES - 1, 0.09f }; //Idle animation data for enemies
	const Dim2Df COINS_ORIGIN = { 4.f, 5.f };
}