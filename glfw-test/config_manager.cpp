#include "stdafx.h"
#include "config_manager.h"

namespace GM
{
	
	pugi::xml_parse_result m_system_config;
	pugi::xml_document m_system_doc;

	int LoadConfig(const char* filename)
	{
		m_system_config = m_system_doc.load_file(filename);
		cout << "xml loaded: "<<filename<<" -> "<<m_system_config.description()<<endl;
		return EXIT_SUCCESS;
	}
	const pugi::char_t* GetValue(const char* name)
	{
		return m_system_doc.child(name).child_value();
	}
}