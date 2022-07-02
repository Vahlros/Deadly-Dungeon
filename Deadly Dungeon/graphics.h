#pragma once
#include "data.h"

/*Animation of a sprite
Requirements: The animated object must have a sprite with a
texture that's a horizontal list of possible individual textures
*/
struct Animation
{
	//Data
	const AnimationData* data{}; //Animation data containing main stats

	//Counters
	char currentFrame = 0; //Current frame of the animation
	float frameTimer = 0; //Counts game frames to keep track of when to change the animation frame

	//Updates animation
	void UpdateAnimation(sf::Sprite& sprite, const float& elapsed);

	//Initiates a new animation
	void Init(const AnimationData* animData);
};

//Animation for tiles on the map
struct AnimatedTiles
{
	//SFML
	sf::Sprite sprite{};
	sf::IntRect globalRect = { 0, 0, 0, 0 };
	sf::FloatRect localRect = { 0.f, 0.f, 0.f, 0.f };

	//Structs
	Animation anim{};
};

//Functions
/* Requirements:
- tileTexture with exact size of the tile being drawn to it (Different sizes for tiles and doors)
- position is in pixels
How it works:
- Uses tile's data to draw it onto a texture at a specific location
- Useful when drawing varying tiles onto a texture */
void DrawTileOntoTexture(const sf::Image& spritesheet, const Tile& tile, sf::Texture& tileTexture, sf::Texture& texture, const Dim2Di& position);