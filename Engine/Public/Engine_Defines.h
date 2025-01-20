#pragma once
#pragma warning (disable : 4251)


#include <d3d9.h>      // d3d9
#include <d3dx9.h>     // d3dx9 의 dll 은 SDK 설치해야함 


#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;


#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"


using namespace Engine;


#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#pragma comment(lib, "../../EngineSDK/Lib/Imgui.lib")


#define EPSILON 0.01f

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG