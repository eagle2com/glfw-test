#pragma once
#include "stdafx.h"

namespace GM
{
	int LoadConfig(const char* filename);
	const pugi::char_t* GetValue(const char* name);

}