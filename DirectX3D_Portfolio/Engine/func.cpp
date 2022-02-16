#include "pch.h"
#include "func.h"
#include "CEventManager.h"
#include "CKeyManager.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CCollider2D.h"
#include "CTransform.h"
#include "CCore.h"

void SaveWStringToFile(const wstring& _str, FILE* _file)
{
	int len = (int)_str.length();
	SaveToFile(&len, _file);
	SaveToFile(_str.data(), _file, (UINT)len);
}

void LoadWStringFromFile(wstring& _str, FILE* _file)
{
	wchar_t buffer[255] = {};
	int len = 0;
	LoadFromFile(&len, _file);
	LoadFromFile(buffer, _file, (UINT)len);
	_str = buffer;
}

wstring StringToWString(string _str)
{
	return wstring(_str.begin(), _str.end());
}

string WStringToString(wstring _str)
{
	return string(_str.begin(), _str.end());
}

void ChangeSceneMode(SCENE_MODE _mode)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::CHANGE_SCENE_MODE;
	evn.lParam = (DWORD_PTR)_mode;
	CEventManager::GetInst()->AddEvent(evn);
}

bool IsMouseClicked(MOUSE_BUTTON _mouseButton, bool _hold)
{
	KEY type = KEY::KEY_END;
	if (MOUSE_BUTTON::LEFT == _mouseButton)
		type = KEY::LBUTTON;
	else if (MOUSE_BUTTON::RIGHT == _mouseButton)
		type = KEY::RBUTTON;

	if (_hold && KEY_HOLD(type))
	{
		return true;
	}

	if (!_hold && KEY_TAP(type))
	{
		return true;
	}

	if (KEY_AWAY(type))
	{
		return false;
	}

	return false;
}

void DragNDrop(CGameObject* _target)
{
	static CGameObject* target = nullptr;

	if (nullptr != target && target != _target)
		return;

	if (IsMouseClicked(MOUSE_BUTTON::LEFT, true) && _target->Collider2D()->IsObjectHovered())
	{
		target = _target;
		float zPos = target->Transform()->GetLocalPos().z;
		Vec2 mousePos = CKeyManager::GetInst()->GetMousePos();;
		Vec2 resolution = CCore::GetInst()->GetWindowResolution();

		mousePos = Vec2(mousePos.x - resolution.x / 2.f, -(mousePos.y - resolution.y / 2.f));

		target->Transform()->SetLocalPos(Vec3(mousePos.x, mousePos.y, target->Transform()->GetLocalPos().z));
	}

	if (!IsMouseClicked(MOUSE_BUTTON::LEFT, true))
	{
		target = nullptr;
	}
}

Matrix GetRotationMatrix(Vec3 _lookAt)
{
	// Right Vector ±¸ÇÏ±â	
	_lookAt.Normalize();
	Vec3 right = Vec3(0.f, 1.f, 0.f).Cross(_lookAt);
	right.Normalize();

	Vec3 up = _lookAt.Cross(right);
	up.Normalize();

	Matrix rotationMatrix = XMMatrixIdentity();
	rotationMatrix._11 = right.x; rotationMatrix._12 = right.y; rotationMatrix._13 = right.z;
	rotationMatrix._21 = up.x; rotationMatrix._22 = up.y; rotationMatrix._23 = up.z;
	rotationMatrix._31 = _lookAt.x; rotationMatrix._32 = _lookAt.y; rotationMatrix._33 = _lookAt.z;

	return rotationMatrix;
}

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _matrix)
{
	Matrix matrix;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			matrix.m[i][j] = (float)_matrix.Get(i, j);
		}
	}

	return matrix;
}

int GetSizeOfFormat(DXGI_FORMAT _format)
{
	int retByte = 0;

	switch (_format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		retByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		retByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		retByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		retByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		retByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		retByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		retByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		retByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		retByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		retByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}
	return retByte / 8;
}

void ChangeScene(CScene* _nextScene)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::CHANGE_SCENE;
	evn.lParam = (DWORD_PTR)_nextScene;
	CEventManager::GetInst()->AddEvent(evn);
}

int IntersectRay(Vec3(&_vertices)[3], Vec3 _start, Vec3 _dir, Vec3& _crossPoint, float& _result)
{
	Vec3 edge[2] = { (Vec3) 0.f, (Vec3) 0.f };

	edge[0] = _vertices[1] - _vertices[0];
	edge[1] = _vertices[2] - _vertices[0];
	
	Vec3 normal = edge[0].Cross(edge[1]).Normalize();
	float b = normal.Dot(_dir);
	
	Vec3 w0 = _start - _vertices[0];
	float a = -normal.Dot(w0);
	float distRatio = a / b;
	
	_result = distRatio;
	
	Vec3 ray = _start + distRatio * _dir;
	
	_crossPoint = ray;
	
	float uu, uv, vv, wu, wv, inverseD;
	uu = edge[0].Dot(edge[0]);
	uv = edge[0].Dot(edge[1]);
	vv = edge[1].Dot(edge[1]);
	
	Vec3 w = ray - _vertices[0];
	wu = w.Dot(edge[0]);
	wv = w.Dot(edge[1]);
	inverseD = uv * uv - uu * vv;
	inverseD = 1.f / inverseD;
	
	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.f || 1.f < u)
	{
		return 0;
	}
	
	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.f || 1.f < (u + v))
	{
		return 0;
	}
	
	return 1;
}