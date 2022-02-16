#pragma once

class Obj;
class Texture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	VIBRATION,
	NONE
};

struct tCamEffect
{
	CAM_EFFECT effect;
	float endTime;
	float curTime;
};

class Camera
{
	SINGLE(Camera)
private :
	Vector2 m_look;
	Vector2 m_diff;
	Vector2 m_prev;
	Vector2 m_vibe;
	Obj* m_target;

	float m_speed;

	float m_stageLeftEnd;
	float m_stageRightEnd;
	float m_stageTopEnd;
	float m_stageBottomEnd;

	bool m_bVibration;

	Texture* m_veil;
	list<tCamEffect> m_listEffect;
	BYTE m_alpha;

public :
	void init();
	void update();
	void render(HDC _dc);
	void SetLookAt(Vector2 _look) { m_look = _look; }
	void SetTarget(Obj* _target) { m_target = _target; }
	void SetStageEnd(float _left, float _top, float _right, float _bottom)
	{ 
		m_stageLeftEnd = _left;
		m_stageRightEnd = _right;
		m_stageTopEnd = _top;
		m_stageBottomEnd = _bottom;
	}
	Vector2 GetLookAt() { return m_look; }
	Vector2 GetDiff() { return m_diff; }
	Vector2 GetRenderPos(Vector2 _pos) { return _pos - m_diff; }
	Vector2 GetRealPos(Vector2 _pos) { return _pos + m_diff; }
	Obj* GetTarget() { return m_target; }
	void AddCamEffect(CAM_EFFECT _effect, float _time) { m_listEffect.push_back(tCamEffect{ _effect, _time, 0.f }); }
	void DeleteAllEffect();
	void SetVibration(Vector2 _range, float _time);
	void StopVibration();

private :
	void MoveCamera();
	void UpdateEffect();
};

