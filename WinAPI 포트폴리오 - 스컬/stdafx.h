// header.h: ǥ�� �ý��� ���� ����
// �Ǵ� ������Ʈ Ư�� ���� ������ ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
#include <commdlg.h>
// C ��Ÿ�� ��� �����Դϴ�.
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