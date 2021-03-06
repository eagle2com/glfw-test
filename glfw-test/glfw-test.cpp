#include "stdafx.h"
#include "render_manager.h"
#include "factory.h"
#include "game_object.h"
#include "event_manager.h"
#include "SparseList.h"
#include "SparseList.cpp"
#include <glm\glm.hpp>


int main()
{
	GM::Init();
	IGameObject *triangle,*cube;

	/*
	while(true)
	{
		string ev;
		cout << ">";
		cin >> ev;
		triangle = GM::Factory::CreateObject("triangle");
		//cube = GM::Factory::CreateObject("cube");
		triangle->SetParam("pos",new glm::vec3(0,0,0));
		GM::Event::SendEvent(GM::Event::GetID(ev),nullptr,false);
		GM::Event::Tick();
		GM::Event::Tick();
		triangle->Destroy();
		//cube->Destroy();
		GM::Factory::Reset();
	}*/

	triangle = GM::Factory::CreateObject("triangle");
	GM::Factory::Obj2Bin("res/meshes/cube");
	GM::Launch();
	
	
    return 0;
}