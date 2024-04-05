#pragma once

#include <cmath>

#ifdef _MSVC_LANG
	#if _MSVC_LANG >= 202002L
		#include <numbers>
		inline constexpr float PI = std::numbers::pi_v<float>;
	#else
		#include <corecrt_math_defines.h>
		static constexpr float PI = M_PI;
	#endif
#else
	#if __cplusplus >= 202002L
		#include <numbers>
		inline constexpr float PI = std::numbers::pi_v<float>;
	#else
		#include <corecrt_math_defines.h>
		static constexpr float PI = M_PI;
	#endif
#endif

inline constexpr float EPSILON = 1e-5f;

float RadToDeg(const float rad);
float DegToRad(const float deg);