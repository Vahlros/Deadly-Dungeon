#include "input.h"

void Input::ProcessEvents(sf::RenderWindow& window)
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		//Exit application
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				escapePressed = true;
			}
			else if (event.key.code == sf::Keyboard::E)
			{
				ePressed = true;
			}
			else if (event.key.code == sf::Keyboard::W)
			{
				wHeld = true;
			}
			else if (event.key.code == sf::Keyboard::A)
			{
				aHeld = true;
			}
			else if (event.key.code == sf::Keyboard::S)
			{
				sHeld = true;
			}
			else if (event.key.code == sf::Keyboard::D)
			{
				dHeld = true;
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				wHeld = false;
			}
			else if (event.key.code == sf::Keyboard::A)
			{
				aHeld = false;
			}
			else if (event.key.code == sf::Keyboard::S)
			{
				sHeld = false;
			}
			else if (event.key.code == sf::Keyboard::D)
			{
				dHeld = false;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				leftClickPressed = true;
				leftClickHeld = true;
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				rightClickPressed = true;
				rightClickHeld = true;
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				leftClickHeld = false;
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				rightClickHeld = false;
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			mousePosition =  Dim2Df(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
		}
	}
}

void Input::Clear()
{
	ePressed = false;
	leftClickPressed = false;
	rightClickPressed = false;
	escapePressed = false;
}