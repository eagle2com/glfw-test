#pragma once
/*Public header of game object*/

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <map>

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
};