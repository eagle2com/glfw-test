#pragma once
#include "stdafx.h"

namespace GM
{
	int Init();
	void Exit();
	bool isRunning();
	void Launch();
	UINT32 GetModel(std::string);
}
