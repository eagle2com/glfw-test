#pragma once
#include "stdafx.h"

class CModel
{
public:
	CModel(void);
	~CModel(void);
	bool Load(S_POINT* vertices,int n_vertex, S_FACE* faces, int n_face);
	void Draw(void);	//maybe with some additional params later on?
	bool isLoaded(void);
	void Init(void);
	void LoadDebugTriangle();


protected:
	GLuint VertexArrayID;
	GLuint VertexBuffer;
	GLuint elementBuffer;
	

private:
	bool m_isLoaded;
};

