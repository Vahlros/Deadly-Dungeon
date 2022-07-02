#pragma once
#include "data.h"
#include "graphics.h"

//Data used for room generation
struct RoomData
{
	char type = 0;
	std::vector<std::vector<unsigned char>> tilemap = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
};

//An interactive game room
struct Room
{
	//Data
	const RoomData* data{}; //Room data

	//Main stats
	char ID = 0; //Which
	sf::IntRect rect = { 0, 0, 0, 0 }; //Where the room is on the global map, in tiles
	std::vector<sf::IntRect> doorsList = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }; //List of door positions

	//Counters
	int doorCounter = 0; //How many doors are in this room

	//Bools
	bool isOpen = true; //If doors are open or not
	bool hasAnimatedTiles = false; //If the room has animations or not
	bool visible = false; //If the room is within the rendered map area

	//Structs
	std::vector<AnimatedTiles> animatedTiles; //Vector of all the animated tiles in this room

	//Initializes the room
	void Init(GameData& game, const int& roomNumber, const Dim2Di& position);

	//Finds doors in this room
	void FindDoors();

	//Check if this door tile is part of an already found door
	bool CheckIfFoundDoor(std::vector<sf::IntRect>& doorsList, const int& doorCounter, const int& x, const int& y);

	//Changes variables to dimensions of the room
	void GetTypeDimensions(int& width, int& height);

	//Draws the tile onto the game map
	void TileDrawing(GameData& game, const int& x, const int& y, const char& tileID);

	//Gets booleans for current tile
	void TileDrawingBools(const Tile& tile, bool& requiresFloor, bool& wallside, bool& walltop, bool& corner, bool& right);

	//Draws a random floor at a specific locations
	void DrawRandomFloor(GameData& game, const int& x, const int& y);

	//Alters the collision map based on the tile
	void AlterCollisionMap(GameData& game, const int& x, const int& y, const char& tileID);

	//Checks for animated tiles
	void CheckForAnimatedTiles(GameData& game, const int& x, const int& y, const char& tileID);

	//Updates animated tiles
	void UpdateAnimatedTiles(const GameData& game, sf::RenderWindow& window);

	//Finds enemy spawn locations
	void FindSpawners();
};

//Game constants
namespace GC
{
	//Room: General
	const char DOOR_TILE_WIDTH = 4;
	const char DOOR_TILE_HEIGHT = 2;
	//Room: General
	const char ROOM_NUM = 3; //Number of rooms
	const char ROOM_MAX_SIZE = 32; //Max size of a room, in tiles
	const char ROOM_MAX_DOORS = 4; //Max doors in a room
	//Room: Types
	const Dim2Di ROOM_TYPE_0 = { 32, 32 };
	const Dim2Di ROOM_TYPE_1 = { 16, 16 };
	const Dim2Di ROOM_TYPE_2 = { 32, 16 };
	const Dim2Di ROOM_TYPE_3 = { 16, 32 };
	//Room: Tilemaps
	const std::vector<RoomData> ROOM_LIST = {
		{1, {{54, 23, 52, 22, 64, 64, 64, 64, 22, 52, 52, 52, 23, 52, 52, 55},
			{14, 19, 02, 16, 64, 64, 64, 64, 16, 02, 04, 02, 18, 02, 02, 15},
			{49, 27, 32, 24, 32, 38, 39, 32, 24, 32, 32, 32, 26, 32, 32, 47},
			{49, 32, 32, 32, 32, 32, 32, 32, 36, 32, 32, 32, 33, 32, 32, 47},
			{49, 32, 35, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 47},
			{49, 32, 32, 32, 32, 33, 32, 32, 38, 32, 32, 32, 32, 39, 32, 47},
			{49, 32, 32, 36, 32, 32, 32, 32, 32, 32, 37, 35, 32, 32, 32, 47},
			{49, 32, 32, 32, 32, 32, 37, 32, 32, 32, 32, 32, 32, 32, 32, 47},
			{49, 34, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 34, 32, 32, 47},
			{49, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 47},
			{49, 32, 32, 32, 37, 35, 32, 32, 32, 39, 32, 32, 32, 32, 32, 47},
			{49, 32, 32, 32, 38, 39, 32, 32, 32, 32, 32, 32, 35, 32, 32, 47},
			{49, 32, 37, 32, 32, 32, 32, 33, 32, 36, 32, 32, 32, 32, 32, 47},
			{59, 52, 52, 52, 52, 52, 52, 30, 65, 65, 65, 65, 30, 52, 52, 61},
			{01, 02, 04, 02, 02, 05, 02, 16, 65, 65, 65, 65, 16, 02, 02, 03},
			{00, 00, 00, 00, 00, 00, 00, 25, 49, 36, 32, 47, 25, 00, 00, 00}}},
		{1, {{54, 23, 52, 23, 52, 23, 52, 22, 64, 64, 64, 64, 22, 52, 23, 55},
			{14, 18, 02, 18, 02, 18, 02, 16, 64, 64, 64, 64, 16, 02, 18, 15},
			{49, 26, 32, 26, 32, 26, 32, 24, 32, 32, 32, 32, 24, 32, 26, 47},
			{49, 32, 40, 32, 32, 32, 33, 32, 32, 34, 32, 35, 32, 32, 32, 47},
			{49, 32, 34, 37, 35, 32, 32, 40, 38, 32, 32, 32, 40, 38, 39, 47},
			{49, 32, 32, 38, 39, 32, 32, 32, 32, 32, 33, 32, 34, 32, 32, 47},
			{49, 32, 36, 40, 32, 34, 32, 39, 32, 40, 32, 32, 32, 32, 32, 47},
			{49, 32, 35, 32, 32, 32, 32, 32, 32, 37, 35, 32, 32, 33, 32, 47},
			{49, 32, 40, 32, 32, 38, 32, 36, 32, 38, 39, 32, 40, 32, 32, 47},
			{49, 32, 32, 32, 32, 32, 32, 40, 32, 32, 32, 32, 36, 32, 32, 47},
			{49, 36, 32, 38, 32, 32, 32, 32, 32, 32, 32, 34, 32, 32, 32, 47},
			{49, 32, 40, 32, 32, 36, 32, 34, 32, 32, 38, 32, 32, 39, 32, 47},
			{49, 32, 32, 33, 32, 32, 32, 32, 40, 32, 32, 32, 40, 32, 32, 47},
			{49, 32, 32, 32, 40, 32, 37, 35, 32, 32, 34, 32, 32, 36, 32, 47},
			{59, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 61},
			{01, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 03}}},
		{1, {{54, 52, 23, 52, 23, 52, 23, 52, 52, 23, 52, 23, 52, 23, 52, 55},
			{14, 02, 19, 02, 19, 02, 19, 05, 02, 19, 02, 19, 02, 19, 02, 15},
			{49, 36, 27, 32, 27, 32, 27, 32, 32, 27, 32, 27, 36, 27, 32, 47},
			{49, 32, 32, 37, 32, 34, 32, 32, 33, 32, 32, 34, 38, 32, 32, 47},
			{49, 32, 32, 33, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 47},
			{49, 32, 32, 32, 32, 32, 34, 32, 35, 32, 32, 32, 32, 33, 32, 47},
			{49, 32, 32, 32, 32, 32, 32, 32, 32, 36, 32, 32, 32, 32, 32, 47},
			{49, 32, 32, 34, 32, 32, 32, 33, 32, 32, 34, 32, 32, 32, 32, 47},
			{49, 38, 32, 32, 32, 32, 32, 39, 32, 32, 32, 32, 32, 32, 32, 47},
			{49, 32, 36, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 36, 32, 47},
			{49, 32, 32, 32, 34, 32, 32, 36, 32, 32, 33, 32, 32, 39, 32, 47},
			{49, 32, 32, 33, 35, 32, 32, 32, 32, 32, 37, 32, 32, 32, 34, 47},
			{49, 34, 32, 32, 32, 32, 34, 32, 33, 32, 32, 36, 32, 32, 32, 47},
			{49, 32, 36, 32, 32, 32, 32, 32, 32, 32, 32, 34, 32, 32, 32, 47},
			{59, 52, 52, 52, 65, 65, 65, 65, 52, 52, 52, 52, 52, 52, 52, 61},
			{01, 02, 05, 02, 65, 65, 65, 65, 02, 02, 02, 04, 02, 02, 02, 03}}} };

	//Tile: Tile list ranges
	const unsigned char DOOR_START_NUM = TILE_NUM - 3; //Where the doors start on the tile list
	const Dim2Di WALL_RANGE = { 1, 20 }; //Where the wall tiles are in the list
	const Dim2Di FLOOR_RANGE = { 32, 39 }; //Where the floor tiles are in the list
	const Dim2Di WALL_SIDE_RANGE = { 45, 50 }; //Where the wall sides are in the list
	const Dim2Di WALL_TOP_RANGE = { 51, 57 }; //Where the wall tops are in the list
	const Dim2Di WALL_CORNER_RANGE = { 58, 61 }; //Where the wall corners are in the list
	//Tile: Tile list enum
	enum TILES {
		T_DARKNESS, T_WALL_LEFT, T_WALL_MID, T_WALL_RIGHT, T_WALL_HOLE_1, T_WALL_HOLE_2, T_WALL_BANNER_BLUE, T_WALL_BANNER_GREEN,
		T_WALL_BANNER_YELLOW, T_WALL_BANNER_RED, T_WALL_CORNER_LEFT, T_WALL_CORNER_RIGHT, T_WALL_CORNER_FRONT_LEFT, T_WALL_CORNER_FRONT_RIGHT,
		T_WALL_IN_CORNER_MID_LEFT, T_WALL_IN_CORNER_MID_RIGHT, T_WALL_COLUMN_MID, T_COLUMN_MID, T_LAVA_FOUNTAIN_MID, T_WATER_FOUNTAIN_MID, T_WALL_GOO,
		T_COLUMN_TOP, T_WALL_COLUMN_TOP, T_FOUNTAIN_TOP, T_WALL_COLUMN_BASE, T_COLUMN_BASE, T_LAVA_FOUNTAIN_BASIN, T_WATER_FOUNTAIN_BASIN, T_WALL_GOO_FLOOR,
		T_EDGE, T_WALL_COLUMN_TOP_FLOOR, T_FOUNTAIN_TOP_FLOOR, T_FLOOR_1, T_FLOOR_2, T_FLOOR_3, T_FLOOR_4, T_FLOOR_5, T_FLOOR_6, T_FLOOR_7, T_FLOOR_8,
		T_ENEMY_SPAWNER, T_FLOOR_SPIKES, T_FLOOR_LADDER, T_HOLE, T_STAIRS, T_WALL_SIDE_TOP_RIGHT, T_WALL_SIDE_TOP_LEFT, T_WALL_SIDE_MID_RIGHT,
		T_WALL_SIDE_FRONT_RIGHT, T_WALL_SIDE_MID_LEFT, T_WALL_SIDE_FRONT_LEFT, T_WALL_TOP_LEFT, T_WALL_TOP_MID, T_WALL_TOP_RIGHT, T_WALL_CORNER_TOP_LEFT,
		T_WALL_CORNER_TOP_RIGHT, T_WALL_IN_CORNER_T_TOP_LEFT, T_WALL_IN_CORNER_T_TOP_RIGHT, T_WALL_CORNER_BOTTOM_LEFT, T_WALL_IN_CORNER_L_TOP_LEFT,
		T_WALL_CORNER_BOTTOM_RIGHT, T_WALL_IN_CORNER_L_TOP_RIGHT, T_BLANK_1, T_BLANK_2, T_DOOR_UP, T_DOOR_DOWN
	};
}