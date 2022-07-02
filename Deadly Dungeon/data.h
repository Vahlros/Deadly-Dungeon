#pragma once
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include "SFML/Graphics.hpp"

using Dim2Df = sf::Vector2f;
using Dim2Di = sf::Vector2i;

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

//Data for current game session
struct GameData
{
	//Window and camera
	Dim2Di screenResolution = { 0, 0 }; //Resolution of the player's screen
	float scaling = 0.f; //Sprite scaling, varies based on the screen
	sf::IntRect cameraRect = { 0, 0, 0, 0 }; //Global position and area of the camera, in pixels
	sf::View camera{}; //View for the window, zoomed in so that the map and sprites can be drawn in pixels

	//States
	char gameState = 0; //Current game state
	char input = 0; //Keyboard or controller

	//Vectors
	std::vector<sf::Texture> textures; //Vector of all textures
	std::vector<std::vector<char>> collisionMap; //Vector of map collision data

	//Images
	sf::Image spritesheetImg{}; //Spritesheet in Image format, for texture.loadFromImage()

	//Sprites
	sf::Sprite mapSprite{}; //Sprite for the environment

	//Timers
	float elapsed = 0.f; //Time elapsed since last frame

	//Pointers
	bool* playerHit = nullptr; //If the player has just been hit

	//Initializes game session
	void Init(sf::RenderWindow& window);

	//Renders the map onto the camera, based on player position
	void RenderMap(sf::RenderWindow& window, const Dim2Df position);
};

//Game constants
namespace GC
{
	//Enums
	enum GAME_STATE { MAIN_MENU, PLAYING, WIN, LOSE }; //Current game state
	enum DIRECTIONS { NORTH, EAST, SOUTH, WEST }; //NESW directions
	enum TEXTURE_LIST {
		SPRITESHEET_TEXTURE, MAP_FLOOR_TEXTURE, TILE_TEXTURE, DOOR_TEXTURE, DOOR_OPEN_TEXTURE, WALL_SIDE_TEXTURE, WALL_TOP_TEXTURE, WATER_FOUNTAIN_TEXTURE,
		LAVA_FOUNTAIN_TEXTURE, KNIGHT_TEXTURE, IMP_TEXTURE, L_DEMON_TEXTURE, ABERRANT_TEXTURE, G_DEMON_TEXTURE, FIRE_SKULL_TEXTURE
	};
	enum PLAYER_INPUT { KEYBOARD, GAMEPAD }; //Player input states
	enum ROOM_TYPES { R32X32, R16X16, R32X16, R16X32 }; //Room types
	enum ATTACK_INPUTS { FIRST_ATTACK, SECOND_ATTACK }; //Attack inputs
	enum COLLISION_TYPES {
		C_FREE_MOVEMENT, C_WALL, C_WALL_TOP, C_WALL_SIDE_LEFT, C_WALL_SIDE_RIGHT, C_WALL_TOP_BOTTOM_LEFT, C_WALL_TOP_BOTTOM_RIGHT, C_FOUNTAIN_TOP, C_FOUNTAIN_BASIN,
		C_COLUMN_TOP, C_COLUMN_BASE, C_CORNER_BOTTOM_LEFT, C_CORNER_BOTTOM_RIGHT
	}; //Collision map types
	enum ANIMATION { IDLE = 0, MOVE = 4, DODGE = 8 }; //Animation frames
	enum ENEMY_ID { ID_IMP, ID_LESSER_DEMON, ID_ABERRANT, ID_GREATER_DEMON }; //Enemy IDs

	//Screen: General
	const float SCALE_1080 = 5.f; //View zoom at 1080p
	const float SCALE_1440 = 6.4f; //View zoom at 1440p
	const float SCALE_2160 = 10.f; //View zoom at 2160p
	const unsigned char FRAMERATE = 60; //Game refresh rate
	const float APPROX_ELAPSED = 1.f / (float)GC::FRAMERATE; //Approximate elapsed time

	//Textures: General
	const char NUM_TEXTURES = 15; //Number of textures used in the game

	//Tile: General
	const char TILE_SIZE = 16; //Tile size, in pixels
	const Dim2Di DOOR_TILESIZE = { 64, 32 }; //Door size, in pixels
	const unsigned char TILE_NUM = 67; //Total number of tiles
	//Tile: Tile dimensions
	const char WALL_SIDE_WIDTH = 5; //Width of a wall side, in pixels
	const char WALL_TOP_HEIGHT = 4; //Width of a wall top, in pixels#
	const char FOUNTAIN_BASIN_HEIGHT = 9;
	const char FOUNTAIN_TOP_HEIGHT = 7;
	const char COLUMN_BASE_HEIGHT = 5;
	const char COLUMN_TOP_HEIGHT = 12;
	const char DOOR_OPEN_WIDTH = 7;
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
		{45, "Wall Side Top Right", {0, 112, 16, 16} },
		{46, "Wall Side Top Left", {16, 112, 16, 16} },
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
		{62, "Blank", {0, 426, 16, 16}},
		{63, "Blank", {0, 426, 16, 16}},
		{64, "Door Up", {16, 208, 64, 32}},
		{65, "Door Down", {16, 240, 64, 32}},
		{66, "Door Open", {80, 208, 32, 32}}
	};
	
	//Map: General
	const unsigned char MAP_SIZE_TILES = 128; //Maximum size of the map, in tiles
	const short MAP_SIZE_PIXELS = MAP_SIZE_TILES * TILE_SIZE; //Maximum size of map, in pixels

	//Player: General
	const Dim2Df START_POSITION = { 684.f, 684.f }; //Start position
	const float PLAYER_HIT_INVULNERABILITY = 2.f; //How long the player is invulnerable after being hit
	const float PLAYER_DODGE_INVULNERABILITY = 0.5f; //Invulnerability while dodging
	//Player: Animation
	const unsigned char PLAYER_ANIM_FRAMES = 9; //Number of frames
	const AnimationData PLAYER_ANIM_IDLE = { IDLE, MOVE - 1, 0.12f }; //Idle animation data for the player
	const AnimationData PLAYER_ANIM_MOVE = { MOVE, DODGE - 1, 0.12f }; //Idle animation data for the player
	//Player: Knight
	const Dim2Di KNIGHT_DIMENSIONS = { 16, 22 }; //Dimensions of the player texture
	const sf::IntRect KNIGHT_ANIM_RECT = { 128, 106, 144, KNIGHT_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect KNIGHT_BODY_RECT = { 0, 6, 16, 16 }; //Where the character's body is on the un-scaled sprite
	const Dim2Di KNIGHT_BODY_CENTRE = { 8, 14 }; //Where the centre of the character's body is on the un-scaled sprite
	
	//Enemy: General
	const float ATTACK_COOLDOWN = 1.f; //Cooldown timer between attacks
	const char MAX_ENEMIES = 10; //Maximum number of enemies
	//Enemy: Animation
	const unsigned char ENEMY_ANIM_FRAMES = 8; //Number of frames
	const AnimationData ENEMY_ANIM_IDLE = { IDLE, MOVE - 1, 0.12f }; //Idle animation data for enemies
	const AnimationData ENEMY_ANIM_MOVE = { MOVE, DODGE - 1, 0.12f }; //Idle animation data for enemies
	const AnimationData ENEMY_ANIM_BITE = { IDLE, MOVE - 1, 0.3f }; //Bite animation for demons
	//Enemy: Imp
	const unsigned char IMP_HEALTH = 2; //Imp health
	const Dim2Di IMP_DIMENSIONS = { 16, 14 }; //Dimensions of the imp texture
	const sf::IntRect IMP_ANIM_RECT = { 369, 50, 128, IMP_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect IMP_BODY_RECT = { 4, 0, 10, IMP_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Di IMP_BODY_CENTRE = { 8, 9 }; //Where the centre of the character's body is on the un-scaled sprite
	//Enemy: Lesser Demon
	const unsigned char L_DEMON_HEALTH = 3; //Lesser Demon health
	const Dim2Di L_DEMON_DIMENSIONS = { 16, 23 }; //Dimensions of the lesser demon texture
	const sf::IntRect L_DEMON_ANIM_RECT = { 369, 329, 128, L_DEMON_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect L_DEMON_BODY_RECT = { 2, 0, 11, L_DEMON_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Di L_DEMON_BODY_CENTRE = { 8, 14 }; //Where the centre of the character's body is on the un-scaled sprite
	//Enemy: Aberrant
	const unsigned char ABERRANT_HEALTH = 4; //Lesser Demon health
	const Dim2Di ABERRANT_DIMENSIONS = { 16, 18 }; //Dimensions of the lesser demon texture
	const sf::IntRect ABERRANT_ANIM_RECT = { 368, 302, 128, ABERRANT_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect ABERRANT_BODY_RECT = { 3, 0, 10, ABERRANT_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Di ABERRANT_BODY_CENTRE = { 8, 12 }; //Where the centre of the character's body is on the un-scaled sprite
	//Enemy: Greater Demon
	const unsigned char G_DEMON_HEALTH = 8; //Lesser Demon health
	const Dim2Di G_DEMON_DIMENSIONS = { 32, 34 }; //Dimensions of the lesser demon texture
	const sf::IntRect G_DEMON_ANIM_RECT = { 16, 366, 256, G_DEMON_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect G_DEMON_BODY_RECT = { 5, 2, 10, G_DEMON_DIMENSIONS.y }; //Where the character's body is on the un-scaled sprite
	const Dim2Di G_DEMON_BODY_CENTRE = { 17, 20 }; //Where the centre of the character's body is on the un-scaled sprite

	//Attack: General
	const unsigned char MAX_MOTIONS = 2; //Maximum number of motions per attack
	const char DEFAULT_DAMAGE = 1; //Default damage value

	//Weapon: General
	const unsigned char MAX_ATTACKS = 2; //Maximum number of attacks per weapon
	const float WEAPON_HOVER = 0.5f; //Hover distance from centre, in tiles
	const float WEAPON_HOVER_ROTATION = 30.f; //Hover rotation, in degrees

	//Projectiles: General
	const short MAX_PROJECTILES = 256;
	//Projectiles: Fiery Skull
	const unsigned char FIRE_SKULL_FRAMES = 6; //Number of frames
	const AnimationData FIRE_SKULL_ANIM = { 0, FIRE_SKULL_FRAMES - 1, 0.09f }; //Idle animation data for the player
	const Dim2Di FIRE_SKULL_DIMENSIONS = { 6, 6 }; //Dimensions of the player texture
	const sf::IntRect FIRE_SKULL_ANIM_RECT = { 293, 341, 36, FIRE_SKULL_DIMENSIONS.y }; //Where the animation is on the spritesheet
	const sf::IntRect FIRE_SKULL_BODY_RECT = { 0, 0, 6, 6 }; //Where the character's body is on the un-scaled sprite
	const Dim2Di FIRE_SKULL_BODY_CENTRE = { 2, 2 }; //Where the centre of the character's body is on the un-scaled sprite

	//Collision
	const char CHECK_ATTACK_COLLISION_RANGE = TILE_SIZE * 4; //The distance between two origins in which collisions will be checked

	//Knockback
	const float KNOCKBACK_TIMER = 0.25f; //How long an enemy is knocked back for
	const float KNOCKBACK_INVULNERABILITY = 0.15f; //How long an entity is invulnerable for in knockback

	//Colours
	const sf::Color PLAYER_HIT_COLOUR = sf::Color(51, 102, 255); //Softer blue than default
	const sf::Color PLAYER_PROJECTILE_COLOUR = sf::Color(153, 204, 0); //Softer green than default
}