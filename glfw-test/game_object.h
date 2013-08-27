#pragma once
#include <vector>
#include "Param.h"
#include <functional>
#include <algorithm>
#include <iostream>
#include <map>
#include <glm\glm.hpp>


using std::cout;
using std::endl;

class IGameObject;

struct FunctionHandle
{
	FunctionHandle(int ev, int fn): event_id(ev), function_id(fn){}
	int event_id;
	int function_id;
};

typedef std::function<bool(void*)> CallbackFn; //a callback function returns a bool to see if the event is "consumed"

typedef void (*SendEventPtr)(unsigned int, void*,bool);
typedef FunctionHandle* (*SubscribePtr)(std::string, CallbackFn);
typedef void (*UnSubPtr)(FunctionHandle*);
typedef unsigned int (*GetIdPtr)(std::string);


//game object interface definition
class IGameObject
{
public:
	IGameObject(): m_sendevenetptr(NULL), m_subscribeptr(NULL)
	{
	}
	virtual ~IGameObject()
	{
		cout << "deleted object"<<endl;
		//tell the event manager to erase callbacks
		for_each( m_functionhandles.begin(), m_functionhandles.end(), [this] (FunctionHandle* handle)
		{
			this->UnSub(handle);
		} );
	}

	virtual void Tick() = 0;
	virtual void Init() = 0;

	void SetFunctions(SendEventPtr send, SubscribePtr sub, UnSubPtr unsub, GetIdPtr idptr)
	{
		m_sendevenetptr = send;
		m_subscribeptr = sub;
		m_unsubptr = unsub;
		m_getidptr = idptr;
	}
	void Subscribe(std::string name, CallbackFn fn)
	{
		if(m_subscribeptr)
		{
			FunctionHandle* handle = m_subscribeptr(name,fn);
			m_functionhandles.push_back(handle);
		}
		else
			throw;
	}

	void UnSub(FunctionHandle* handle)
	{
		if(m_unsubptr)
			m_unsubptr(handle);
		else
			throw;
	}

	unsigned int GetID(std::string name)
	{
		if(m_getidptr)
		{
			unsigned int id = m_getidptr(name);
			//cout << "[object].GetID: "<<id<<endl;
			return id;
		}
		else
			throw;
	}

	void SendEvent(unsigned int ID, void* data, bool free_data = false)
	{
		if(m_sendevenetptr)
		{
			//cout << "[object].SendEvent: "<<ID<<endl;
			m_sendevenetptr(ID,data,free_data);
			return;
		}
		else
			throw;
	}

	void Destroy()
	{
		delete this;
	}

	void* GetParam(std::string name)
	{
		std::map<std::string,CParam*>::iterator it = m_params.find(name);
		if (it != m_params.end())
			return it->second->m_data;
		else
			return NULL;
	}

	void SetParam(std::string name, void* p)
	{
		std::map<std::string,CParam*>::iterator it = m_params.find(name);
		if(it != m_params.end())
		{
			delete it->second->m_data;
			it->second->m_data = p;
		}
		else
			m_params[name] = new CParam(name,p);
	}

private:
	std::map<std::string,CParam*> m_params;
	std::vector<FunctionHandle*> m_functionhandles; //vector holding all the handles we have to remove from the event manager after destroying the object
	SendEventPtr m_sendevenetptr;
	SubscribePtr m_subscribeptr;
	UnSubPtr m_unsubptr;
	GetIdPtr m_getidptr;


protected:

	glm::vec3 m_pos,m_rot,m_scl;
	
};

typedef  void (IGameObject::*IGameObjectFn)(); //pointer to a void member function of IGameObject

#define CONNECT(c_name,e_name,e_fun) Subscribe(e_name,std::bind(&c_name::e_fun,this,std::placeholders::_1));

#define GAME_OBJECT(class_name) \
	extern "C" __declspec(dllexport) class_name* __cdecl create_object(){return new class_name;}

struct ObjectRequest
{
	ObjectRequest(std::string n, IGameObject* ob = NULL):name(n),object(ob){}
	std::string name;
	IGameObject* object;
};