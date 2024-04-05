#include "Math.h"

float RadToDeg(const float rad)
{
	static constexpr float k = 180 / PI;
	return k * rad;
}

float DegToRad(const float deg)
{
	static constexpr float k = PI / 180;
	return k * deg;
}