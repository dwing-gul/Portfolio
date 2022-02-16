#pragma once
#include "global.h"
#include "CResourceManager.h"

template <typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
		}
	}

	_map.clear();
}

template <typename T>
void Safe_Delete_Vec(vector<T>& _vector)
{
	for (size_t i = 0; i < _vector.size(); ++i)
	{
		if (nullptr != _vector[i])
			delete _vector[i];
	}

	_vector.clear();
}

template <typename T, UINT size>
void Safe_Delete_Arr(T* (&arr)[size])
{
	for (UINT i = 0; i < size; ++i)
	{
		if (nullptr != arr[i])
		{
			delete arr[i];
			arr[i] = nullptr;
		}
	}
}

template <typename T>
void Safe_Delete_Ptr(T* _ptr)
{
	if (nullptr != _ptr)
	{
		delete _ptr;
		_ptr = nullptr;
	}
}

void SaveWStringToFile(const wstring& _str, FILE* _file);
void LoadWStringFromFile(wstring& _str, FILE* _file);

template <typename T>
void SaveToFile(T* _data, FILE* _file, UINT _elementCount = 1)
{
	fwrite(_data, sizeof(T), (size_t)_elementCount, _file);
}

template <typename T>
void LoadFromFile(T* _data, FILE* _file, UINT _elementCount = 1)
{
	fread(_data, sizeof(T), (size_t)_elementCount, _file);
}

wstring StringToWString(string _str);
string WStringToString(wstring _str);

class CRes;

template <typename T>
void SaveResourceToFile(Ptr<T> _res, FILE* _file)
{
	UINT check = 0;

	if (nullptr != _res)
	{
		check = 1;
	}
	SaveToFile(&check, _file);

	if (nullptr != _res)
	{
		SaveWStringToFile(_res->GetKey(), _file);
		SaveWStringToFile(_res->GetRelativePath(), _file);
	}
}

template <typename T>
void LoadResourceFromFile(Ptr<T>& _res, FILE* _file)
{
	UINT check = 0;
	LoadFromFile(&check, _file);

	if (check)
	{
		wstring key, relativePath;
		LoadWStringFromFile(key, _file);
		LoadWStringFromFile(relativePath, _file);

		_res = CResourceManager::GetInst()->Load<T>(key, relativePath);
	}
	else
	{
		_res = nullptr;
	}
}

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _matrix);
int GetSizeOfFormat(DXGI_FORMAT _format);

class CScene;
void ChangeScene(CScene* _nextScene);
void ChangeSceneMode(SCENE_MODE _mode);

bool IsMouseClicked(MOUSE_BUTTON _mouseButton = MOUSE_BUTTON::LEFT, bool _hold = false);

class CGameObject;

// ui 드래그앤드롭을 위한 함수.
// 현재 물체의 움직임의 크기와 마우스의 움직임의 크기가 안 맞는 문제가 있음
void DragNDrop(CGameObject* _target);

Matrix GetRotationMatrix(Vec3 _lookAt);

int IntersectRay(Vec3 (&_vertices)[3], Vec3 _start, Vec3 _dir, Vec3& _crossPoint, float& _result);