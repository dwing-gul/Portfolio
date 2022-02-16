#pragma once

#include <stdlib.h>
#include <Windows.h>
#include <assert.h>
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
using std::make_pair;
#include <string>
using std::string;
using std::wstring;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "SimpleMath.h"
#include <wrl.h>
using namespace Microsoft::WRL;

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "extern.h"
#include "func.h"