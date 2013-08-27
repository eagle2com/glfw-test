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
	void Init(void);
	glm::mat4 GetMatrix();

protected:
	GLuint VertexArrayID;
	GLuint VertexBuffer;
	glm::mat4 m_matrix;

private:
	bool m_isLoaded;
};

