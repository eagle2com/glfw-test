// triangle.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "../glfw-test/game_object.h"
#include <iostream>
#include <SFML\Window.hpp>

class CTriangle: public IGameObject
{
public:
	void Tick()
	{
	}
	
	void Init()
	{
		//CONNECT(CTriangle,"sfml",onEvent)
		CONNECT(CTriangle,"event1",onEvent1);
		CONNECT(CTriangle,"event2",onEvent2);
		CONNECT(CTriangle,"sfml",onsfmlEvent);
	}

	bool onsfmlEvent(void* data)
	{
		assert(data != nullptr);
		sf::Event* ev = (sf::Event*)data;
		if(ev->type == sf::Event::KeyPressed)
			std::cout << "[Triangle] key code: "<<ev->key.code<<std::endl;
		return false;
	}

	bool onEvent1(void* data)
	{
		cout << "triangle received event1" <<endl;
		SendEvent(GetID("triangle"),NULL);
		return false;
	}

	

	bool onEvent2(void* data)
	{
		cout << "triangle received event2" <<endl;
		return false;
	}
	
};


GAME_OBJECT(CTriangle)