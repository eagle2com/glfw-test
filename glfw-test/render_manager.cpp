#include "stdafx.h"
#include "render_manager.h"
#include "constants.h"
#include "config_manager.h"
#include "factory.h"
#include "event_manager.h"
#include "SparseList.h"
#include "Model.h"

namespace GM
{
	int OpenWindow();
	void Update();
	void Render();
	void WebUpdate();

	sf::Window* m_window = NULL;
	bool m_running = false;
	sf::Thread m_renderthread(&Render);
	sf::Thread m_updatethread(&Update);
	sf::Thread m_webthread(&WebUpdate);

	std::thread *p_renderthread;
	std::thread *p_updatethread;
	std::thread *p_webthread;
	std::recursive_mutex render_mutex;

	//sf::Mutex render_mutex;

	std::map<std::string, UINT32> m_model_map;
	CSparseList<CModel> m_model_list(10);
	UINT32 m_model_id = 0;

	bool need_gl_context = false;
	bool needed_gl_context = false;

	

	///REMOVE THIS
	CModel model1;
	///

	int Init()
	{
		
		LoadConfig("config/system.xml");
		Factory::Init();
		return EXIT_SUCCESS;
	}

	int OpenWindow()
	{
		sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 0;
		settings.majorVersion = 3;
		settings.minorVersion = 3;

		cout << "opengl version: "<<settings.majorVersion<<"."<<settings.minorVersion;
		const pugi::char_t* hc = GetValue("h");
		const pugi::char_t* wc = GetValue("w");
		//cout << "setting "<<wc<<"x"<<hc<<" mode"<<endl;

		int style = 0;
		if(atoi(GetValue("full"))==1)
		{
			style = sf::Style::Fullscreen;
		}
		else
			style = sf::Style::Default;

		m_window = new sf::Window(sf::VideoMode(atoi(wc), atoi(hc)), "OpenGL", style, settings);
		settings = m_window->getSettings();
		glewExperimental = GL_TRUE;
		glewInit();
		cout << " -> "<<settings.majorVersion<<"."<<settings.minorVersion<<endl;
		
		return EXIT_SUCCESS;
	}

	void Exit()
	{
		m_running = false;
		//m_updatethread.wait();
		//m_renderthread.wait();
		//m_webthread.wait();
		p_renderthread->join();
	}

	bool isRunning()
	{
		return m_running;
	}

	void Launch()
	{
		std::chrono::milliseconds dura(30);
		UINT64 previous_time;

		static UINT32 sfml_ev_id = Event::GetID("sfml");
		static UINT32 update_ev_id = Event::GetID("update");
		m_running = true;
		OpenWindow();
		m_window->setActive(false);
		p_renderthread = new std::thread(Render);
		//m_updatethread.launch();
		//m_webthread.launch();
		sf::Event event;

		
		
		sf::Clock main_clock;
		previous_time = main_clock.getElapsedTime().asMicroseconds();

		while(m_running)
		{
			double dt = 1.0/(main_clock.getElapsedTime().asMicroseconds()-previous_time);
			while (m_window->pollEvent(event))
			{
				Event::SendEvent(sfml_ev_id,new sf::Event(event),true);
				if (event.type == sf::Event::Closed)
				{
					// end the program
					m_running = false;
				}
				else if (event.type == sf::Event::Resized)
				{
					// adjust the viewport when the window is resized
					glViewport(0, 0, event.size.width, event.size.height);
				}
				else if(event.type == sf::Event::KeyPressed)
				{
					if(event.key.code == sf::Keyboard::Escape)
					{
						m_running = false;
					}
						
					if (event.key.code == sf::Keyboard::L)
					{
						
						render_mutex.lock();

						model1.Init();
						model1.Load(string("null"));
						render_mutex.unlock();
					}
					


					cout<<"[GM] Key pressed :"<<event.key.code<<endl;
				}
			}
			Event::SendEvent(update_ev_id,&dt,false);
			GM::Event::Tick();
			
			std::this_thread::sleep_for(dura);
		}
		Exit();
	}

	void Update()
	{
		std::chrono::milliseconds dura(30);
		while(m_running)
		{

			std::this_thread::sleep_for(dura);
		}
	}

	void Render()
	{
		std::chrono::milliseconds dura(30);
		/* This makes our buffer swap syncronized with the monitor's vertical refresh */
		m_window->setVerticalSyncEnabled(true);
		m_window->setActive(true);
		glClearColor ( 0.1f, 0.1f, 0.1f, 1.0f );	
		cout << "started render loop"<<endl;
		while (m_running)
		{
			
			render_mutex.lock();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			///REMOVE THIS
			if (model1.isLoaded())
			{
				model1.Draw();
			}
			///

			m_window->display();
			render_mutex.unlock();
				
			std::this_thread::sleep_for(dura);
		}
		cout << "render stopping"<<endl;
	}

	void WebUpdate()
	{
		while(m_running)
		{
			sf::sleep(sf::milliseconds(30));
		}
	}

	UINT32 GetModel(std::string name)
	{
		std::map<std::string, UINT32>::iterator it = m_model_map.find(name);
		if(it != m_model_map.end())
			return it->second;

		CModel* model = new CModel();
		UINT32 id = m_model_list.Push(model);
		m_model_map[name] = id;
		return id;
	}
}