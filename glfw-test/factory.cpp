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
	CSparseList<IGameObject> m_game_objects(10);

	std::map<std::string, UINT32> m_model_map;
	CSparseList<CModel> m_model_list(10);
	UINT32 m_model_id = 0;

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
			obh->factory = reinterpret_cast<CreateFunc>(::GetProcAddress(obh->dll_handle,"create_object"));
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
		assert(data != nullptr);
		cout << "on request new received"<<endl;
		ObjectRequest* request = (ObjectRequest*)data;
		request->object = GM::Factory::CreateObject(request->name);
		assert(request->object != nullptr);
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

	UINT32 LoadModelFromBin(std::string name)
	{
		std::map<std::string, UINT32>::iterator it = m_model_map.find(name);
		if (it != m_model_map.end())
		{
			cout << name << " was in cache" << endl;
			return it->second;
		}
			
		cout << "loading " << name << endl;
		CModel* model = new CModel();
		std::ifstream bin_file(name + ".bin", std::ios::binary | std::ios::in);
		assert(bin_file);
		int n_vertex = 0;
		int n_face = 0;
		bin_file.read((char*) &n_vertex, sizeof(int) );
		bin_file.read((char*) &n_face, sizeof(int) );
		S_POINT* vertices = (S_POINT*) malloc(sizeof(S_POINT)*n_vertex);	assert(vertices);
		S_FACE* faces = (S_FACE*) malloc(sizeof(S_FACE)*n_face);			assert(faces);
		bin_file.read((char*) vertices, sizeof(S_POINT)*n_vertex);
		bin_file.read((char*) faces, sizeof(S_POINT)*n_face);

		UINT32 id = m_model_list.Push(model);
		model->Init();
		model->Load(vertices, n_vertex, faces, n_face);
		m_model_map[name] = id;
		return id;
	}
	
	CModel* GetModel(UINT32 id)
	{
		return m_model_list.Get(id);
	}
	

	bool Obj2Bin(std::string name)
	{
		std::ifstream obj_file(name+".obj", std::ios::in);
		assert(obj_file);

		std::vector<S_POINT> vertices;
		std::vector<S_FACE> faces;

		string line;
		while (getline(obj_file,line))
		{
			if (line[0] == '#')					//ignore .obj comments
				continue;

			else if (line[0] == 'v')			//found vertex
			{
				float x, y, z;
				sscanf_s(line.c_str(), "%f %f %f", &x, &y, &z);
				vertices.push_back(S_POINT(x, y, z));
			}
			else if (line[0] == 'f')			//found face
			{
				UINT16 a, b, c;
				sscanf_s(line.c_str(), "%u %u %u", &a, &b, &c);
				faces.push_back(S_FACE(a, b, c));
			}
			//cout << line << endl;	
		}
		obj_file.close();
		cout << "Found " << vertices.size() << " vertices and " << faces.size() << " faces" << endl;

		std::ofstream bin_file(name + ".bin",std::ios::binary | std::ios::out);
		assert(bin_file);
		int n_vertex = vertices.size();
		int n_face = faces.size();
		bin_file.write((char*)&n_vertex, sizeof(int) );
		bin_file.write((char*)  &n_face, sizeof(int) );
		for (auto& vertex : vertices)
		{
			bin_file.write((char*)&vertex,sizeof(S_POINT));
		}
		for (auto& face : faces)
		{
			bin_file.write((char*) &face, sizeof(S_FACE));
		}

		bin_file.close();
		cout << "Successfuly converted obj to bin" << endl;
		return true;
	}
	
}
}