#include "stdafx.h"
#include "Model.h"


CModel::CModel(void) : m_isLoaded(false)
{
	
}


CModel::~CModel(void)
{
}

void CModel::Draw(void)
{
	if (!m_isLoaded) 
		throw string("Attempting to draw an uninitialized model");

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}

void CModel::Init(void)
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

bool CModel::Load(string& name)
{
	if (m_isLoaded)
		return false;

	static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1,&VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	m_isLoaded = true;
	cout << "loaded model: " << name << endl;

	return true;
}

bool CModel::isLoaded(void)
{
	return m_isLoaded;
}