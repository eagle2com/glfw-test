// camera.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "../glfw-test/game_object.h"

class CCamera :
	public IGameObject
{
public:
	CCamera::CCamera()
	{
		
	}


	CCamera::~CCamera()
	{
	}

	void CCamera::Init()
	{
		CONNECT(CCamera, "sfml", onsfmlEvent);
		m_Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		m_View = glm::lookAt(
			glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
		
	}

	bool CCamera::onsfmlEvent(void* data)
	{
		assert(data != nullptr);
		sf::Event* ev = (sf::Event*)data;
		cout << "camera event" << endl;

		return false;	//by default do not consume the event
	}

	glm::mat4 CCamera::GetMatrix()
	{
		return m_Projection*m_View;
	}

	void CCamera::Tick()
	{

	}

private:
	glm::mat4 m_Projection;
	glm::mat4 m_View;
};

GAME_OBJECT(CCamera)


