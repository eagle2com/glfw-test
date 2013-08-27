#include "stdafx.h"
#include "Param.h"


CParam::CParam(std::string name, void* data) : m_data(data), m_name(name)
{
}


CParam::~CParam(void)
{
}
