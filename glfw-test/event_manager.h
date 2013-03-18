#pragma once
#include "stdafx.h"
#include "GameEvent.h"
#include "game_object.h"

namespace GM
{
namespace Event
{
	unsigned int GetID(std::string);
	void Tick();
	void SendEvent(unsigned int ID, void* data, bool free_data);
	FunctionHandle* Subscribe(std::string name, CallbackFn);
	void UnSub(FunctionHandle*);
	unsigned int Dispatch(CGameEventHandle*);
}
}