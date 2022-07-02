#pragma once
#include <math.h>
#include <cmath>
#include "data.h"

//Game constants
namespace GC
{
	//Angles
	const float RADS_90DEGREES = 1.570796f; //East
	const float RADS_180DEGREES = 3.141592f; //South
	const float RADS_270DEGREES = 4.712388f; //West
	const float RADS_360DEGREES = 6.283185f; //North, also 0 degrees
	const float RADS_5DEGREES = 0.082673f; //For calculating specific degrees in radians
	const float RADS_45DEGREES = 0.785398f; //For specific directional movement
	const float DEGREES_360 = 360.f; //360 degrees
	const float DEGREES_315 = 315.f; //315 degrees
	const float DEGREES_270 = 270.f; //270 degrees
	const float DEGREES_225 = 225.f; //225 degrees
	const float DEGREES_180 = 180.f; //180 degrees
	const float DEGREES_135 = 135.f; //135 degrees
	const float DEGREES_90 = 90.f; //90 degrees
	const float DEGREES_45 = 45.f; //45 degrees
	const float ZERO = 0.f; //Zero
	const float SIN_COS_45_DEGREES = 0.707107f; //Sin or cos of 45 degrees
}

//Calculates vector using coords
//Returns: Dim2Df vector (between 2 points, global bounds)
Dim2Df CalculateVectorBetweenPoints(const Dim2Df& origin, const Dim2Df& target);

//Calculates angle from 2D dimensions
//Returns: DirectionalAngle (angle from clockwise NESW axis)
DirectionalAngle CalculateDirectionalAngleFromVector(const Dim2Df& vector);

//Calculates 2D vector using angle and 1D magnitude
//Requirements: Angle must be in radians
//Returns: Dim2Df vector (of a magnitude e.g. speed, knockback)
Dim2Df CalculateVectorOfMagnitude(const DirectionalAngle& dirAngle, const float& magnitude);

//Gets a directional angle from a full 360 range angle
DirectionalAngle GetDirectionalAngleFrom360Angle(const float& angle, const bool& isRadians);

//Gets angle up to 2 PI without direction, default from North
//Returns: float angle (in degrees)
float GetFullAngleInRads(const DirectionalAngle& dirAngle);

//Gets angle up to 360 degrees without direction, default from North
//Returns: float angle (in degrees)
float GetFullAngleInDegrees(const DirectionalAngle& dirAngle);

//Constrains angle between 2 PI and 0
void ConstrainAngle(float& angle, const bool& isRadians);

//Converts an angle from radians to degrees
void ConvertToDegrees(float& angle);

//Converts an angle from degrees to radians
void ConvertToRadians(float& angle);

//Calculates vector translation of a circular motion
//Requirements: angle in radians
//Returns: Dim2Df translation
Dim2Df CalculateCircularMotionVector(const float& radius, const float& angle);

//Calculates vector translation of a circular motion from an origin
//Requirements: angle in radians
//Returns: Dim2Df translation
Dim2Df CalculateCircularMotionPosition(const Dim2Df& origin, const float& radius, const float& angle);

//Calculates the 1D magnitude of a 2D vector
//Requirements: components of vector must be floats
//Returns: float magnitude
float CalculateMagnitudeOfVector(const Dim2Df& vector);