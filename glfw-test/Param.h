#pragma once
#include <string>

using std::string;

class CParam
{
public:
	friend class IGameObject;
	CParam(std::string, void*);
	~CParam(void);
	void* m_data;

protected:
	
	string m_name;
};

