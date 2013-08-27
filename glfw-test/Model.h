#pragma once
#include "stdafx.h"

class CModel
{
public:
	CModel(void);
	~CModel(void);
	bool Load(string& name);
	void Draw(void);	//maybe with some additional params later on?
	bool isLoaded(void);

protected:
	GLuint VertexArrayID;
	GLuint VertexBuffer;

private:
	bool m_isLoaded;
};

