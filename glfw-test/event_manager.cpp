#include "stdafx.h"
#include "event_manager.h"
#include "game_object.h"
#include "GameEvent.h"
#include <functional>

typedef std::map<std::string, unsigned int> IDMap;

namespace GM
{
namespace Event
{
	//first and second queue of events (to avoid being stuck in event update)
	std::queue<CGameEventHandle*> m_events0;
	std::queue<CGameEventHandle*> m_events1;
	int m_current_queue = 0;
	
	int m_event_id = 0;
	int m_function_id = 0;
	std::vector<std::map<int,CallbackFn>*> m_subscribers;
	IDMap m_event_map;

	unsigned int GetID(std::string name)
	{
		unsigned int id = 0;
		IDMap::iterator it = m_event_map.find(name);
		if(it != m_event_map.end())
			id = it->second;
		else //event type does not exist yet, let's create an ID for it
		{
			m_event_map[name] = m_event_id;		//map it ...
			m_subscribers.push_back(new std::map<int,CallbackFn>);  // ... and create a corresponding list of subscribers
			id = m_event_id++;
		}
		//std::cout << "getid["<<name<<"]: "<<id<<std::endl;
		return id;
	}

	void Tick()
	{
		std::queue<CGameEventHandle*>& cqueue = m_current_queue==0?m_events0:m_events1;
		m_current_queue = !m_current_queue;
		while(!cqueue.empty())
		{
			CGameEventHandle* ev = cqueue.front();
			//std::cout << "dispatching "<<ev->m_ID<<std::endl;
			Dispatch(ev);
			cqueue.pop();
		}
	}

	void SendEvent(unsigned int ID, void* data, bool free_data)
	{
		//std::cout << "SendEvent: " <<ID<<std::endl;
		std::queue<CGameEventHandle*>& cqueue = m_current_queue==0?m_events0:m_events1;
		CGameEventHandle* ev = new CGameEventHandle(ID,data,free_data);
		cqueue.push(ev);
		//std::cout << "ID: "<<ev->m_ID<<std::endl;
	}

	FunctionHandle* Subscribe(std::string name, CallbackFn fn)
	{
		unsigned int id = GetID(name);
		std::map<int,CallbackFn> *list = NULL;
		if(id >= m_subscribers.size())
		{
			list = new std::map<int,CallbackFn>;
			m_subscribers[id] = list;
		}
		else
			list = m_subscribers[id];

		(*list)[m_function_id] = fn;

		return new FunctionHandle(id,m_function_id++);
	}

	void UnSub(FunctionHandle* handle)
	{
		std::map<int,CallbackFn> *list = m_subscribers[handle->event_id];
		list->erase(handle->function_id);
	}
	
	unsigned int Dispatch(CGameEventHandle* ev) 
	{
		bool consumed = false;

		std::map<int,CallbackFn> *list = m_subscribers[ev->m_ID];
		for_each(list->begin(), list->end(),[ev,&consumed] (std::pair<int,CallbackFn> pair) //closures FTW!!!
		{
			if(!consumed && pair.second(ev->m_data))
				consumed = true;
		}
		);
		if(ev->m_free_data)
		{
			delete ev->m_data;
			ev->m_data = NULL;
		}
		delete ev;
		ev = NULL;
		return list->size();
	}

}
}