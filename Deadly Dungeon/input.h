#pragma once
#include "SFML/Graphics.hpp"

using Dim2Df = sf::Vector2f;
using Dim2Di = sf::Vector2i;

struct Input
{
	//Bools
	bool wHeld = false;
	bool aHeld = false;
	bool sHeld = false;
	bool dHeld = false;
	bool ePressed = false;
	bool leftClickPressed = false;
	bool leftClickHeld = false;
	bool rightClickPressed = false;
	bool rightClickHeld = false;
	bool escapePressed = false;

	//Mouse position
	Dim2Df mousePosition;

	void ProcessEvents(sf::RenderWindow& window);

	void Clear();
};