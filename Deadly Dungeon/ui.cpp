#include "ui.h"

//Initialize UI
void UI::Init(const sf::Image &spritesheet)
{
	//Text
	font.loadFromFile("EquipmentPro.ttf");

	//Textures
	//healthBar texture etc

	//Sprites
	//healthBar sprite etc
}

//Render all UI elements
void UI::Render(sf::RenderWindow &window)
{
	//Player
	window.draw(healthBarSprite);
	window.draw(itemBarSprite);
}

//Changes the health bar's texture
void UI::UpdateHealthBar()
{

}

//Changes the item bar's texture
void UI::UpdateItemBar()
{

}