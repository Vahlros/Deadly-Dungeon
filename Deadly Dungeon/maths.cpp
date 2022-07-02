#include "maths.h"

//Calculates vector using coords
//Returns: Dim2Df vector (between 2 points, global bounds)
Dim2Df CalculateVectorBetweenPoints(const Dim2Df& origin, const Dim2Df& target)
{
	return { target.x - origin.x, target.y - origin.y };
}

//Calculates angle from 2D dimensions
//Returns: DirectionalAngle (angle from clockwise NESW axis)
DirectionalAngle CalculateDirectionalAngleFromVector(const Dim2Df& vector)
{
	DirectionalAngle dirAngle;

	//Find direction
	if (vector.x > 0)
	{
		if (vector.y > 0)
		{
			dirAngle.direction = GC::EAST;
		}
		else
		{
			dirAngle.direction = GC::NORTH;
		}
	}
	else
	{
		if (vector.y > 0)
		{
			dirAngle.direction = GC::SOUTH;
		}
		else
		{
			dirAngle.direction = GC::WEST;
		}
	}

	Dim2Df absoluteVector;
	absoluteVector.x = abs(vector.x);
	absoluteVector.y = abs(vector.y);

	//Calculate angle based on direction
	if ((dirAngle.direction == GC::NORTH) || (dirAngle.direction == GC::SOUTH))
	{
		dirAngle.angle = std::atan(absoluteVector.x / absoluteVector.y);
	}
	else //East or West
	{
		dirAngle.angle = std::atan(absoluteVector.y / absoluteVector.x);
	}

	return dirAngle;
}

//Calculates 2D vector using angle and 1D magnitude
//Requirements: Angle must be in radians
//Returns: Dim2Df vector (of a magnitude e.g. speed, knockback)
Dim2Df CalculateVectorOfMagnitude(const DirectionalAngle& dirAngle, const float& magnitude)
{
	Dim2Df magVector;

	if ((dirAngle.direction == GC::NORTH) || (dirAngle.direction == GC::SOUTH))
	{
		//Find absolute values
		magVector.x = magnitude * std::sin(dirAngle.angle);
		magVector.y = magnitude * std::cos(dirAngle.angle);

		//Convert to direction
		if (dirAngle.direction == GC::NORTH)
		{
			magVector.y *= -1;
		}
		else //South
		{
			magVector.x *= -1;
		}
	}
	else //East or West
	{
		//Find absolute values
		magVector.y = magnitude * std::sin(dirAngle.angle);
		magVector.x = magnitude * std::cos(dirAngle.angle);

		//Convert to direction
		if (dirAngle.direction == GC::WEST)
		{
			magVector.x *= -1;
			magVector.y *= -1;
		}
	}

	return magVector;
}

//Adds an angle in degrees to the directional angle in radians, then re-formats it
void AddDegreesToDirectionalAngleInRads(DirectionalAngle& dirAngle, const float& angle)
{
	//Convert to degrees and add angle
	dirAngle.angle = GetFullAngleInDegrees(dirAngle);
	dirAngle.direction = GC::NORTH;
	dirAngle.angle += angle;

	//Convert to radians and back to a directional angle
	ConvertToRadians(dirAngle.angle);
	ConstrainAngle(dirAngle.angle, true);
	dirAngle = GetDirectionalAngleFrom360Angle(dirAngle.angle, true);
}

//Gets a directional angle from a full 360 range angle
DirectionalAngle GetDirectionalAngleFrom360Angle(const float& angle, const bool& isRadians)
{
	DirectionalAngle dirAngle = { angle, GC::NORTH };

	if (isRadians)
	{
		if (dirAngle.angle >= GC::RADS_270DEGREES)
		{
			dirAngle.direction = GC::WEST;
			dirAngle.angle -= GC::RADS_270DEGREES;
		}
		else if (dirAngle.angle >= GC::RADS_180DEGREES)
		{
			dirAngle.direction = GC::SOUTH;
			dirAngle.angle -= GC::RADS_180DEGREES;
		}
		else if (dirAngle.angle >= GC::RADS_90DEGREES)
		{
			dirAngle.direction = GC::EAST;
			dirAngle.angle -= GC::RADS_90DEGREES;
		}
	}
	else
	{
		if (dirAngle.angle >= GC::DEGREES_270)
		{
			dirAngle.direction = GC::WEST;
			dirAngle.angle -= GC::DEGREES_270;
		}
		else if (dirAngle.angle >= GC::DEGREES_180)
		{
			dirAngle.direction = GC::SOUTH;
			dirAngle.angle -= GC::DEGREES_180;
		}
		else if (dirAngle.angle >= GC::DEGREES_90)
		{
			dirAngle.direction = GC::EAST;
			dirAngle.angle -= GC::DEGREES_90;
		}
	}

	return dirAngle;
}

//Gets angle up to 2 PI without direction, default from North
//Returns: float angle (in degrees)
float GetFullAngleInRads(const DirectionalAngle& dirAngle)
{
	float angle = dirAngle.angle;

	if (dirAngle.direction == GC::WEST)
	{
		angle += GC::RADS_270DEGREES;
	}
	else if (dirAngle.direction == GC::SOUTH)
	{
		angle += GC::RADS_180DEGREES;
	}
	else if (dirAngle.direction == GC::EAST)
	{
		angle += GC::RADS_90DEGREES;
	}

	return angle;
}

//Gets angle up to 360 degrees without direction, default from North
//Returns: float angle (in degrees)
float GetFullAngleInDegrees(const DirectionalAngle& dirAngle)
{
	float angle = GetFullAngleInRads(dirAngle);
	ConvertToDegrees(angle);

	return angle;
}

//Ensures validity of angle
void ConstrainAngle(float& angle, const bool& isRadians)
{
	if (isRadians)
	{
		if (angle > GC::RADS_360DEGREES)
		{
			angle -= GC::RADS_360DEGREES;
		}
		else if (angle < 0.f)
		{
			angle += GC::RADS_360DEGREES;
		}
	}
	else
	{
		if (angle > GC::DEGREES_360)
		{
			angle -= GC::DEGREES_360;
		}
		else if (angle < 0.f)
		{
			angle += GC::DEGREES_360;
		}
	}
}

//Converts an angle from radians to degrees
void ConvertToDegrees(float& angle)
{
	angle *= (GC::DEGREES_180 / GC::RADS_180DEGREES);
}

//Converts an angle from degrees to radians
void ConvertToRadians(float& angle)
{
	angle *= (GC::RADS_180DEGREES / GC::DEGREES_180);
}

//Calculates vector translation of a circular motion, where 0 is north
//Requirements: angle in radians
//Returns: Dim2Df translation
Dim2Df CalculateCircularMotionVector(const float& radius, const float& angle)
{
	return { radius * std::cos(angle - GC::RADS_90DEGREES), radius * std::sin(angle - GC::RADS_90DEGREES) };
}

//Calculates vector translation of a circular motion from an origin
Dim2Df CalculateCircularMotionPosition(const Dim2Df& origin, const float& radius, const float& angle)
{
	return { origin.x + (radius * std::cos(angle - GC::RADS_90DEGREES)), origin.y + (radius * std::sin(angle - GC::RADS_90DEGREES)) };
}

//Calculates the 1D magnitude of a 2D vector
//Requirements: components of vector must be floats
//Returns: float magnitude
float CalculateMagnitudeOfVector(const Dim2Df& vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}