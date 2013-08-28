#include "IGameObject.h"
#include "../glfw-test/Param.h"
//#include <glm\glm.hpp>


IGameObject::IGameObject(void) : m_sendevenetptr(nullptr), m_subscribeptr(nullptr)
{
}
IGameObject::~IGameObject()
{
	cout << "deleted object" << endl;
	//tell the event manager to erase callbacks
	for_each(m_functionhandles.begin(), m_functionhandles.end(), [this](FunctionHandle* handle)
	{
		this->UnSub(handle);
	});
}

void IGameObject::SetFunctions(SendEventPtr send, SubscribePtr sub, UnSubPtr unsub, GetIdPtr idptr)
{
	m_sendevenetptr = send;
	m_subscribeptr = sub;
	m_unsubptr = unsub;
	m_getidptr = idptr;
}
void IGameObject::Subscribe(std::string name, CallbackFn fn)
{
	if (m_subscribeptr)
	{
		FunctionHandle* handle = m_subscribeptr(name, fn);
		m_functionhandles.push_back(handle);
	}
	else
		throw;
}

void IGameObject::UnSub(FunctionHandle* handle)
{
	if (m_unsubptr)
		m_unsubptr(handle);
	else
		throw;
}

unsigned int IGameObject::GetID(std::string name)
{
	if (m_getidptr)
	{
		unsigned int id = m_getidptr(name);
		//cout << "[object].GetID: "<<id<<endl;
		return id;
	}
	else
		throw;
}

void IGameObject::SendEvent(unsigned int ID, void* data, bool free_data = false)
{
	if (m_sendevenetptr)
	{
		//cout << "[object].SendEvent: "<<ID<<endl;
		m_sendevenetptr(ID, data, free_data);
		return;
	}
	else
		throw;
}

void IGameObject::Destroy()
{
	delete this;
}

void* IGameObject::GetParam(std::string name)
{
	std::map<std::string, CParam*>::iterator it = m_params.find(name);
	if (it != m_params.end())
		return it->second->m_data;
	else
		return NULL;
}

void IGameObject::SetParam(std::string name, void* p)
{
	std::map<std::string, CParam*>::iterator it = m_params.find(name);
	if (it != m_params.end())
	{
		delete it->second->m_data;
		it->second->m_data = p;
	}
	else
		m_params[name] = new CParam(name, p);
}

