#include "motion.h"
#include "maths.h"

//Initiates motion
void Motion::Init(const GameData& game, const DirectionalAngle& facing, const float& attackSpeed, const int& swingDirection, const bool& followingFacing)
{
	active = true;

	//Get timer, motions should be the same time or not required to be
	if (circular)
	{
		timer = circleData->time / attackSpeed;
	}
	else if (line)
	{
		timer = lineData->time / attackSpeed;
	}
	else if (spin)
	{
		timer = spinData->time / attackSpeed;
	}
	else
	{
		timer = 4.0f; //For testing purposes
	}

	//Get change variables based on direction
	if (facing.direction == GC::NORTH || facing.direction == GC::EAST) //Right
	{
		facingRight = true;

		if (circular)
		{
			circleSpeed = circleData->rotation / timer * swingDirection;
		}
		if (line)
		{
			lineSpeed.x = lineData->translation.x / timer;
			lineSpeed.y = lineData->translation.y / timer;
		}
		if (spin)
		{
			spinSpeed = attackSpeed * spinData->rotation / spinData->time * swingDirection; //Not required to be the same
		}
	}
	else //Left
	{
		facingRight = false;

		if (circular)
		{
			circleSpeed = -circleData->rotation / timer * swingDirection;
		}
		if (line)
		{
			if (followingFacing)
			{
				lineSpeed.x = lineData->translation.x / timer;
				lineSpeed.y = lineData->translation.y / timer;
			}
			else
			{
				lineSpeed.x = -lineData->translation.x / timer;
				lineSpeed.y = -lineData->translation.y / timer;
			}
		}
		if (spin)
		{
			spinSpeed = attackSpeed * -spinData->rotation / spinData->time * swingDirection; //Not required to be the same
		}
	}
}

//Updates motion totals
void Motion::UpdateTotals(const GameData& game)
{
	//Motion end bools
	if (timer < 0.f)
	{
		active = false;
	}
	else if (active)
	{
		//Circular total
		if (circular)
		{
			circleTotal += circleSpeed * game.elapsed;
		}

		//Line total
		if (line)
		{
			lineTotal.x += lineSpeed.x * game.elapsed;
			lineTotal.y += lineSpeed.y * game.elapsed;
		}

		//Spin total
		if (spin)
		{
			spinTotal += spinSpeed * game.elapsed;
		}
	}
}

//Updates the position of the global rect
void Motion::UpdatePosition(sf::Sprite* sprite, const bool& followingFacing, const DirectionalAngle& facing, const float& initialAngle, const float& radius)
{
	Dim2Df lineTranslation = { 0.f, 0.f }, circularTranslation = { 0.f, 0.f };
	float angle;

	//Find translations
	if (line)
	{
		//Find angle in radians
		DirectionalAngle dirAngle = GetDirectionalAngleFrom360Angle(initialAngle, false);
		ConvertToRadians(dirAngle.angle);

		if (followingFacing)
		{
			//Magnitude is x, direction is y
			lineTranslation = CalculateVectorOfMagnitude(dirAngle, lineTotal.x);
			lineTranslation.x *= lineData->translation.y;
			lineTranslation.y *= lineData->translation.y;
		}
		else
		{
			//Magnitude is square root of vector components
			float magnitude = CalculateMagnitudeOfVector(lineTotal);
			lineTranslation = CalculateVectorOfMagnitude(dirAngle, magnitude);
		}
	}
	if (circular)
	{
		//Correct arcs when changing direction
		if (followingFacing && (circular || spin))
		{
			if (facingRight && (facing.direction == GC::WEST || facing.direction == GC::SOUTH))
			{
				facingRight = false;
				if (circular)
				{
					circleSpeed *= -1.f;
					circleTotal *= -1.f;
				}
				if (spin)
				{
					spinSpeed *= -1.f;
					spinTotal *= -1.f;
				}
			}
			else if (!facingRight && (facing.direction == GC::EAST || facing.direction == GC::NORTH))
			{
				facingRight = true;
				if (circular)
				{
					circleSpeed *= -1.f;
					circleTotal *= -1.f;
				}
				if (spin)
				{
					spinSpeed *= -1.f;
					spinTotal *= -1.f;
				}
			}
		}

		//Calculate translation
		angle = initialAngle + circleTotal;
		ConvertToRadians(angle);
		circularTranslation = CalculateCircularMotionVector(radius, angle);
	}

	//Translate globalRect
	sprite->move(lineTranslation.x + circularTranslation.x, lineTranslation.y + circularTranslation.y);
}

//Resets the motion back to original values
void Motion::ResetTotals()
{
	circleTotal = 0.f;
	lineTotal = { 0.f, 0.f };
	spinTotal = 0.f;
}