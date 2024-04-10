#pragma once

#ifdef _MSVC_LANG
	#define CPP_VERSION _MSVC_LANG
#else
	#define CPP_VERSION __cplusplus
#endif

#if CPP_VERSION >= 201703L
	#define CONST inline constexpr
#elif CPP_VERSION >= 201103L
	#define CONST static constexpr
#else
	#define CONST static const
#endif