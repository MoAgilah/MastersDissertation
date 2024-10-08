#pragma once
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

//----------------------------------------------------------------------------
//	some random number functions.
//----------------------------------------------------------------------------

//returns a random integer between x and y
inline int	RandInt(int x, int y) { return rand() % (y - x + 1) + x; }

//returns a random float between zero and 1
inline double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }

//returns a random bool
inline bool RandBool()
{
	if (RandInt(0, 1))
		return true;
	else
		return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }

//-----------------------------------------------------------------------
//
//	some handy little functions
//-----------------------------------------------------------------------

//converts an integer to a string
inline std::string itos(int arg)
{
	std::ostringstream buffer;

	//send the int to the ostringstream
	buffer << arg;

	//capture the string
	return buffer.str();
}

//converts a float to a string
inline std::string ftos(float arg)
{
	std::ostringstream buffer;

	//send the int to the ostringstream
	buffer << arg;

	//capture the string
	return buffer.str();
}

//clamps the first argument between the second two
inline void Clamp(double& arg, double min, double max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}

inline void Clamp(int& arg, int min, int max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}

//rounds a double up or down depending on its value
inline int Rounded(double val)
{
	int    integral = (int)val;
	double mantissa = val - integral;

	if (mantissa < 0.5)
	{
		return integral;
	}
	else
	{
		return integral + 1;
	}
}

//rounds a double up or down depending on whether its
//mantissa is higher or lower than offset
inline int RoundUnderOffset(double val, double offset)
{
	int    integral = (int)val;
	double mantissa = val - integral;

	if (mantissa < offset)
	{
		return integral;
	}
	else
	{
		return integral + 1;
	}
}

/////////////////////////////////////////////////////////////////////
//
//	Point structure
//
/////////////////////////////////////////////////////////////////////
struct SPoint
{
	SPoint() = default;
	SPoint(float a, float b)
		: x(a), y(b)
	{}

	float x;
	float y;
};