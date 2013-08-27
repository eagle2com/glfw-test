#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#include <map>
#include <vector>
#include <queue>
#include <functional>
#include <list>
#include <thread>
#include <mutex>
#include <fstream>
#include <algorithm>

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <SFML\System.hpp>

//xml parser pugixml
#include "pugiconfig.hpp"
#include "pugixml.hpp"

//opengl math library glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <string>
using std::string;

#include "SparseList.h"

struct S_POINT
{
	float x, y, z;
	S_POINT(float x, float y, float z) : x(x), y(y), z(z)
	{

	}
};

struct S_FACE
{
	S_FACE(UINT16 a, UINT16 b, UINT16 c) : a(a), b(b), c(c)
	{}
	UINT16 a, b, c;
};