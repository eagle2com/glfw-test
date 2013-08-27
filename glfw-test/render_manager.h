#pragma once
#include "stdafx.h"

namespace GM
{
	int Init();
	void Exit();
	bool isRunning();
	void Launch();
	UINT32 GetModel(std::string);
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
}
