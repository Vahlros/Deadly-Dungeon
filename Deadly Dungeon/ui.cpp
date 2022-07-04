#include "ui.h"

void UI::Init(const sf::Image &spritesheet)
{
	//Text
	font.loadFromFile("EquipmentPro.ttf");

	//Textures
	//healthBar texture etc

	//Sprites
	//healthBar sprite etc
}

void UI::Render(sf::RenderWindow &window)
{
	//Player
	window.draw(healthBarSprite);
	window.draw(itemBarSprite);
}

void UI::UpdateHealthBar()
{

}