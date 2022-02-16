#pragma once

enum class KEY
{
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	F1,
	F2,
	F3,
	F4, 
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9,
	N0,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	SPACE,
	TAB,
	ESC,
	ENTER,
	LSHIFT,
	LCTRL,
	LALT,

	LBUTTON,
	RBUTTON,

	KEY_END
};

enum class KEY_STATE
{
	TAP,
	HOLD,
	AWAY,
	NONE
};

struct tKey
{
	KEY_STATE	eState;
	bool		bPrevPress;
};

class KeyManager : 
	public Singleton<KeyManager>
{
	SINGLE(KeyManager);
private :
	vector<tKey>	m_vectorKey;
	Vector3			m_mousePos;
	Vector3			m_mousePrevPos;

	Vector2			m_mouseDir;

public :
	void init();
	void update();

	KEY_STATE GetState(KEY _key) { return m_vectorKey[(UINT)_key].eState; }
	Vector3 GetMousePos() { return m_mousePos; }
	Vector2 GetMouseDir() { return m_mouseDir; }

	bool NotPressAnyKey();
};

