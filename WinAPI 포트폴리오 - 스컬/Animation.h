#pragma once

class Animator;
class Texture;

struct tAnimFrame
{
	Texture* texture;
	Vector2 leftTop;
	Vector2 slice;
	Vector2 offset;
	float duration;
};

class Animation
{
private :
	Animator* m_animator;
	wstring m_name;
	vector<tAnimFrame> m_vectorFrame;
	vector<float> m_vectorDuration; // 공격속도 변화로 인한 애니메이션 속도 변경 시 원래 애니메이션 속도를 기록하기 위한 벡터
	int m_curFrameIdx;
	float m_accTime;
	bool m_bFinish;
	bool m_bPlay;

public :
	void Create(const wstring& _name, Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum, float _duration);
	void Delete(int _frameNum);
	void Resetting(Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum);
	void Reset() { m_accTime = 0; m_bFinish = false; m_curFrameIdx = 0; }
	bool IsFirst() {
		if (0 == m_curFrameIdx)
			return true;
		return false;
	}
	void Play() { m_bPlay = true; }
	void Stop() { m_bFinish = true; }
	void ChangeAnim(const wstring& _name);

	tAnimFrame& GetFrame(UINT _frameIdx) 
	{
		if (m_vectorFrame.size() <= _frameIdx)
		{
			MessageBox(nullptr, L"프레임 범위 초과", L"프레임 인덱스 에러", MB_OK);
			return  m_vectorFrame[0];
		}
		else
		{
			return m_vectorFrame[_frameIdx];
		}
	}

	vector<tAnimFrame>& GetAllFrame() { return m_vectorFrame; }

	void SetOffset(Vector2 _offset) { m_vectorFrame[m_curFrameIdx].offset = _offset; }
	void SetDuration(float _duration) { m_vectorFrame[m_curFrameIdx].duration = _duration; }
	void SetFrameIdx(int _idx) { m_curFrameIdx = _idx; }
	void SetAnimSpeed(float _ratio);

	const wstring& GetName() { return m_name; }
	int GetCurFrameIdx() { return m_curFrameIdx; }
	int GetFrameSize() { return (int)m_vectorFrame.size(); }
	bool IsFinish() { return m_bFinish; }

	void lateUpdate();
	void render(HDC _dc);
	void EffectRender(HDC _dc);

	void Save(FILE* _file);
	void Load(FILE* _file);

	CLONE(Animation)

public :
	Animation();
	~Animation();

	friend class Animator;
};

