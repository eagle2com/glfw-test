#pragma once
#include <iostream>
#include <SFML\System.hpp>

class CGameEventHandle
{
public:
	CGameEventHandle(unsigned int ID, void* data,bool free_data = true): m_ID(ID), m_data(data),m_free_data(free_data)
	{
		//std::cout << "Created event: "<<ID<<std::endl;
	}
	~CGameEventHandle(void){}
	unsigned int m_ID;
	void* m_data;
	bool m_free_data;
};

class CGameEvent
{
public:
	virtual ~CGameEvent();
};

class ConSFML: CGameEvent
{
public:
	ConSFML(sf::Event ev):m_event(ev){}
	sf::Event Event(){return m_event;}

private:
	sf::Event m_event;
};