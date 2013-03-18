#include "stdafx.h"
#include "factory.h"
#include "game_object.h"
#include "event_manager.h"

typedef IGameObject* (__cdecl *CreateFunc)();



class GameObjectHandle
{
public:
	GameObjectHandle()
	{
		//cout << "loaded library"<<endl;
	}
	~GameObjectHandle()
	{
		FreeLibrary(dll_handle);
		//cout << "freeing library"<<endl;
	}
	HINSTANCE dll_handle;
	CreateFunc factory;
};

typedef std::map<std::string, GameObjectHandle*> CreateFuncMap;

namespace GM
{
namespace Factory
{
	CreateFuncMap m_create_functions;
	CSparseList<IGameObject> m_game_objects;

	IGameObject* CreateObject(std::string name)
	{
		IGameObject* p  = NULL;
		CreateFuncMap::iterator it = m_create_functions.find(name);
		if(it != m_create_functions.end())
		{
			p = it->second->factory();
			p->SetFunctions(&GM::Event::SendEvent,
				&GM::Event::Subscribe,
				&GM::Event::UnSub,
				&GM::Event::GetID);
			p->Init();
			cout << "created "<<name<<endl;
		}
		else //and now the dll loading fun begins :D
		{
		//	std::cout << "Loading " << name <<endl;
			GameObjectHandle* obh = new GameObjectHandle;
			obh->dll_handle = ::LoadLibraryA((name+std::string(".dll")).c_str());

			if(!obh->dll_handle)
			{
				std::cout << "Could not find " << name << ".dll"<<endl;
				delete obh;
				return NULL;
			}
			
			//obh->factory= reinterpret_cast<CreateFunc>(::GetProcAddress(obh->dll_handle,(std::string("create_" )+name).c_str()));
			obh->factory= reinterpret_cast<CreateFunc>(::GetProcAddress(obh->dll_handle,"create_object"));
			if(!obh->factory)
			{
				std::cout << "Could not load <" << name << "> symbol "<<endl;
				delete obh;
				return NULL;
			}
			m_create_functions[name] = obh;
			//cout << "loaded handle:"<<obh->dll_handle<<endl;
			//std::cout << "Sucess ! "<<endl;
			p = obh->factory();
			p->SetFunctions(&GM::Event::SendEvent,
				&GM::Event::Subscribe,
				&GM::Event::UnSub,
				&GM::Event::GetID);
			p->Init();
			cout << "loaded and created "<<name<<endl;
			
		}
		m_game_objects.Push(p);
		return p;
	}
	
	void Reset()
	{
		//for_each(m_create_functions.begin(),m_create_functions.end(),[](GameObjectHandle* handle){delete handle;});
		
		for(CreateFuncMap::iterator it = m_create_functions.begin(); it != m_create_functions.end(); it++)
		{
			delete it->second;
		}

		m_create_functions.clear();
	}


	void ClearAll()
	{
		Reset();
		
	}
	bool onRequestNew(void* data)
	{
		cout << "on request new received"<<endl;
		ObjectRequest* request = (ObjectRequest*)data;
		request->object = GM::Factory::CreateObject(request->name);
		return true;
	}
	bool onRequestObject(void* data)
	{
		cout << "on request object received, not implemented yet"<<endl;
		return true;
	}

	void Init()
	{
		GM::Event::Subscribe("request_new",CallbackFn(onRequestNew));
		GM::Event::Subscribe("request_object",CallbackFn(onRequestObject));
	}

	
	
}
}