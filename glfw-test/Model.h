#pragma once
#include "stdafx.h"

class CModel
{
public:
	CModel(void);
	~CModel(void);
	bool Load();

protected:
	GLuint VertexArrayID;
	GLuint VertexBuffer;
};

