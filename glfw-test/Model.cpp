#include "stdafx.h"
#include "Model.h"


CModel::CModel(void)
{
	
}


CModel::~CModel(void)
{
}

bool CModel::Load()
{
	static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1,&VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1,&VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	return true;
}