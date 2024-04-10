#pragma once

#include <cmath>

#include "Version.h"

#if CPP_VERSION >= 202002L
	#include <numbers>
	CONST double PI = std::numbers::pi;
#else
	#include <corecrt_math_defines.h>
	CONST double PI = M_PI;
#endif

CONST double EPSILON = 1e-5;

template <typename T>
inline bool IsZero(const T value)
{
	return abs(value) < EPSILON;
}

template <typename T>
inline T RadToDeg(const T rad)
{
	static constexpr T k = static_cast<T>(180 / PI);
	return k * rad;
}

template <typename T>
inline T DegToRad(const T deg)
{
	static constexpr T k = static_cast<T>(PI / 180);
	return k * deg;
}