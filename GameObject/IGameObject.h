#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <map>
#include "../glfw-test/Param.h"

using std::cout;
using std::endl;

class IGameObject;

struct FunctionHandle
{
	FunctionHandle(int ev, int fn) : event_id(ev), function_id(fn){}
	int event_id;
	int function_id;
};

typedef std::function<bool(void*) > CallbackFn; //a callback function returns a bool to see if the event is "consumed"

typedef void (*SendEventPtr)(unsigned int, void*, bool);
typedef FunctionHandle* (*SubscribePtr)(std::string, CallbackFn);
typedef void (*UnSubPtr)(FunctionHandle*) ;
typedef unsigned int (*GetIdPtr)(std::string);
typedef void (*SetParamPtr)(std::string, void*);
typedef void* (*GetParamPtr)(std::string);


class IGameObject
{
public:
	IGameObject();
	virtual ~IGameObject();

	virtual void Tick() = 0;
	virtual void Init() = 0;

	void SetFunctions(SendEventPtr send, SubscribePtr sub, UnSubPtr unsub, GetIdPtr idptr);
	void Subscribe(std::string name, CallbackFn fn);
	void UnSub(FunctionHandle* handle);
	unsigned int GetID(std::string name);
	void SendEvent(unsigned int ID, void* data, bool free_data = false);
	void Destroy();
	void* GetParam(std::string name);
	void SetParam(std::string name, void* p);

private:
	std::map<std::string, CParam*> m_params;
	std::vector<FunctionHandle*> m_functionhandles; //vector holding all the handles we have to remove from the event manager after destroying the object
	SendEventPtr m_sendevenetptr;
	SubscribePtr m_subscribeptr;
	UnSubPtr m_unsubptr;
	GetIdPtr m_getidptr;
};

typedef  void (IGameObject::*IGameObjectFn)(); //pointer to a void member function of IGameObject

#define CONNECT(c_name,e_name,e_fun) Subscribe(e_name,std::bind(&c_name::e_fun,this,std::placeholders::_1));

#define GAME_OBJECT(class_name) \
	extern "C++" __declspec(dllexport) class_name* __cdecl create_object(){ return new class_name; }

struct ObjectRequest
{
	ObjectRequest(std::string n, IGameObject* ob = NULL) : name(n), object(ob){}
	std::string name;
	IGameObject* object;
};