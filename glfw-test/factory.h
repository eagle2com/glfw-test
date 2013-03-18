#pragma once
#include "game_object.h"

namespace GM
{
namespace Factory
{
	IGameObject* CreateObject(std::string name);
	void Reset();
	void ClearAll();
	void Init();
}
}