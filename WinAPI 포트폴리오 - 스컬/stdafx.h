// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <commdlg.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <wchar.h>
#include <crtdbg.h>
#include <vector>
using std::vector;
#include <map>
using std::map;
using std::make_pair;
#include <string>
using std::wstring;
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <list>
using std::list;

#pragma comment(lib, "Msimg32.lib")

#define _CRTDBG_MAP_ALLOC

#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "define.h"
#include "struct.h"
#include "func.h"