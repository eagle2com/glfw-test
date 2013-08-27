#pragma once
#include "game_object.h"
#include "Model.h"

namespace GM
{
namespace Factory
{
	IGameObject* CreateObject(std::string name);
	void Reset();
	void ClearAll();
	void Init();
	UINT32 LoadModelFromBin(std::string name);
	bool Obj2Bin(std::string name);
	CModel* GetModel(UINT32 id);

}
}