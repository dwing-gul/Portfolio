#pragma once
#include "global.h"
#include "ResourceManager.h"

template<typename T>
void Safe_Delete(T& _ptr)
{
	if (nullptr != _ptr)
		delete _ptr;

	_ptr = nullptr;
}

template<typename T1, typename T2>
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

template<typename T>
void Safe_Delete_Vector(vector<T>& _vector)
{
	for (size_t i = 0; i < _vector.size(); ++i)
	{
		if (nullptr != _vector[i])
			delete _vector[i];
	}

	_vector.clear();
}

template<typename T, UINT size>
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

void SaveWStringToFile(const wstring& _str, FILE* _file);
void LoadWStringFromFile(wstring& _str, FILE* _file);

template<typename T>
void SaveToFile(T* _data, FILE* _file, UINT _elementCount = 1)
{
	fwrite(_data, sizeof(T), (size_t)_elementCount, _file);
}

template<typename T>
void LoadFromFile(T* _data, FILE* _file, UINT _elementCount = 1)
{
	fread(_data, sizeof(T), (size_t)_elementCount, _file);
}

class Res;

template<typename T>
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

template<typename T>
void LoadResourceFromFile(Ptr<T>& _res, FILE* _file)
{
	UINT check = 0;
	LoadFromFile(&check, _file);

	if (check)
	{
		wstring key, relativePath;
		LoadWStringFromFile(key, _file);
		LoadWStringFromFile(relativePath, _file);

		_res = ResourceManager::GetInst()->Load<T>(key, relativePath);
	}
	else
	{
		_res = nullptr;
	}
}

void ChangeSceneMode(SCENE_MODE _mode);

class Scene;
void ChangeScene(Scene* _nextScene);

string WStringToString(const wstring& _wstring); 
wstring StringToWString(const string& _string); 

bool IsMouseClicked(MOUSE_BUTTON _mouseButton = MOUSE_BUTTON::LEFT, bool _hold = false);

class GameObject;

void DragNDrop(GameObject* _target);