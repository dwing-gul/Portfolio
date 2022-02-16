#pragma once

class Animation;
class Texture;
class Obj;

class Animator
{
private :
	Obj* m_owner;
	map<wstring, Animation*> m_mapAnim;
	Animation* m_curAnim;
	bool m_bRepeat;

public :
	void CreateAnim(const wstring& _name, Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum, float _duration);
	void DeleteAnim(const wstring& _name, int _frameNum);
	void DeleteAllAnim();
	void ResettingAnim(const wstring& _name, Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum);
	void AdjustAnimPos(const wstring& _name, UINT _frameIdx, Vector2 _offset);
	void PlayAnim(const wstring& _name, bool _repeat);
	void NullCurAnim() { m_curAnim = nullptr; }
	void MoveAnim(const wstring& _name);
	void ChangeAnim(const wstring& _name); // 애니메이션 전환을 위한 함수

	void SetRepeat(bool _repeat) { m_bRepeat = _repeat; }

	Obj* GetObj() { return m_owner; }
	map<wstring, Animation*>& GetAllAnim() { return m_mapAnim; }
	Animation* GetAnim(const wstring& _name);
	Animation* GetCurAnim() { return m_curAnim; }
	wstring GetCurAnimName();
	size_t GetAnimCount() { return m_mapAnim.size(); }

	void lateUpdate();
	void render(HDC _dc);
	void EffectRender(HDC _dc);

	void SaveAnim();
	void LoadAnim();

	CLONE(Animator)

	void Load(const wstring& _relativePath);

private :
	void Save(const wstring& _relativePath);
	

public :
	Animator();
	~Animator();

	friend class Obj;

private :
	Animator(const Animator& _origin);
};

