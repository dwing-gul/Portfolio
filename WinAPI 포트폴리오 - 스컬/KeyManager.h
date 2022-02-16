#pragma once

enum class KEY
{
	LEFT,
	RIGHT,
	UP, 
	DOWN,
	Z,
	X,
	C,
	W,
	A, 
	S,
	D,
	F,
	L,
	N1,
	N2,
	N3,
	F1,
	F2,
	SPACE,
	TAB,
	ESC,
	LBUTTON,
	RBUTTON,
	END
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

class KeyManager
{
	SINGLE(KeyManager)
private :
	vector<tKey> m_vectorKey;
	Vector2 m_mousePos;

public :
	void init();
	void update();

	KEY_STATE GetState(KEY _key) { return m_vectorKey[(UINT)_key].eState; }
	Vector2 GetMousePos() { return m_mousePos; }

	bool NotPressAnyKey();
};

