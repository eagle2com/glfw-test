#pragma once
#include "../glfw-test/game_object.h"
#include <iostream>
#include <functional>


class CCube :
	public IGameObject
{

public:
	CCube(void)
	{
		//std::cout << "a cute cube created !!!"<<std::endl;
	}
	~CCube(void)
	{
	}

	void Tick()
	{
		
	}

	void Init()
	{
		CONNECT(CCube,"triangle",onTriangle);
		CONNECT(CCube,"sfml",onsfmlEvent);
	}

	bool Event1(void* data)
	{
		std::cout << "Event1 received by cube!"<<std::endl;
		SendEvent(GetID("request_new"),new ObjectRequest("triangle"));
		return false;
	}

	bool onsfmlEvent(void* data)
	{
		std::cout << "cube eceived sfml event"<<std::endl;
		return false;
	}

	bool e(void* data)
	{
		std::cout << "e hue hue received by cube ["<<this<<"]"<<std::endl;
		return false;
	}

	bool onTriangle(void* data)
	{
		std::cout << "received from triangle"<<std::endl;
		return false;
	}

	void HandleEvent(int ID, void* data) //obsolete??
	{
	}

};

GAME_OBJECT(CCube)