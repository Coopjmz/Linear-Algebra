#pragma once

#ifdef _DEBUG
	#include <cassert>
	#define Assert assert
#else
	#define Assert
#endif