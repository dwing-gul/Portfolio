#pragma once

class Obj;
class Scene;
class Texture;

void CreateObject(Obj* _obj, GROUP_TYPE _eType);
void DeleteObject(Obj* _obj);

Vector2 RotateVector2(Vector2 _origin, float _degree);

void MenuBridge(WPARAM _menuID);

int CrossVectors(Vector2 _obj1, Vector2 _obj2);

void SaveWString(const wstring& _str, FILE* _file);
void LoadWString(wstring& _str, FILE* _file);

void PlaySoundEffect(const wstring& _key, const wstring& _relativePath, bool _loop, float _pos, float _volume);
void PlayBGM(const wstring& _key, const wstring& _relativePath, bool _loop, float _pos, float _volume);
void StopSound(const wstring& _key);

static HWND hDlg;