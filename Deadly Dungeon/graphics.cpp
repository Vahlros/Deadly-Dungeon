#include "graphics.h"

//Updates animation
void Animation::UpdateAnimation(sf::Sprite& sprite, const float& elapsed)
{
	if (frameTimer > data->frameChange)
	{
		frameTimer = 0.f;

		//Change current frame
		if (currentFrame == data->endFrame)
		{
			currentFrame = data->startFrame;
		}
		else
		{
			currentFrame += 1;
		}

		//Change sprite's texture rect
		sf::IntRect rect = sprite.getTextureRect();
		rect.left = rect.width * currentFrame;
		sprite.setTextureRect(rect);
	}
	else
	{
		frameTimer += elapsed;
	}
}

//Initiates a new animation
void Animation::Init(const AnimationData* animData)
{
	data = animData;
	currentFrame = animData->startFrame;
	frameTimer = 0.f;
}

/* Requirements:
- tileTexture with exact size of the tile being drawn to it (Different sizes for tiles and doors)
- position is in pixels
How it works:
- Uses tile's data to draw it onto a texture at a specific location
- Useful when drawing varying tiles onto a texture */
void DrawTileOntoTexture(const sf::Image& spritesheet, const Tile& tile, sf::Texture& tileTexture, sf::Texture& texture, const Dim2Di& position)
{
	tileTexture.loadFromImage(spritesheet, tile.rect);
	texture.update(tileTexture, position.x, position.y);
}