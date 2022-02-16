#pragma once

struct Vector2
{
	float x, y;

public :
	float length()
	{
		return sqrtf((x * x) + (y * y));
	}

	void normalize()
	{
		float len = length();
		x /= len;
		y /= len;
	}

	float Dot(Vector2 _other)
	{
		return x * _other.x + y * _other.y;
	}

	bool IsZero()
	{
		return (0 == x && 0 == y) ? true : false;
	}

	bool operator == (Vector2 _other)
	{
		return (x == _other.x && y == _other.y) ? true : false;
	}

	bool operator != (Vector2 _other)
	{
		return *this == _other ? false : true;
	}

	Vector2 operator -()
	{
		return Vector2(-x, -y);
	}

	Vector2 operator +(Vector2 _other)
	{
		return Vector2(x + _other.x, y + _other.y);
	}

	Vector2 operator +(int _value)
	{
		return Vector2(x + (float)_value, y + (float)_value);
	}

	Vector2 operator +(float _value)
	{
		return Vector2(x + _value, y + _value);
	}

	void operator +=(Vector2 _other)
	{
		x += _other.x;
		y += _other.y;
	}

	void operator +=(int _value)
	{
		x += float(_value);
		y += (float)_value;
	}

	void operator +=(float _value)
	{
		x += _value;
		y += _value;
	}

	Vector2 operator -(Vector2 _other)
	{
		return Vector2(x - _other.x, y - _other.y);
	}

	Vector2 operator -(int _value)
	{
		return Vector2(x - (float)_value, y - (float)_value);
	}

	Vector2 operator -(float _value)
	{
		return Vector2(x - _value, y - _value);
	}

	void operator -=(Vector2 _other)
	{
		x -= _other.x;
		y -= _other.y;
	}

	void operator -=(int _value)
	{
		x -= float(_value);
		y -= (float)_value;
	}

	void operator -=(float _value)
	{
		x -= _value;
		y -= _value;
	}

	Vector2 operator *(Vector2 _other)
	{
		return Vector2(x * _other.x, y * _other.y);
	}

	Vector2 operator *(int _value)
	{
		return Vector2(x * (float)_value, y * (float)_value);
	}

	Vector2 operator *(float _value)
	{
		return Vector2(x * _value, y * _value);
	}

	void operator *=(Vector2 _other)
	{
		x *= _other.x;
		y *= _other.y;
	}

	void operator *=(int _value)
	{
		x *= float(_value);
		y *= (float)_value;
	}

	void operator *=(float _value)
	{
		x *= _value;
		y *= _value;
	}

	Vector2 operator /(Vector2 _other)
	{
		if (0 == _other.x && 0 == _other.y)
			assert(nullptr);

		return Vector2(x / _other.x, y / _other.y);
	}

	Vector2 operator /(int _value)
	{
		if (0 == _value)
			assert(nullptr);

		return Vector2(x / (float)_value, y / (float)_value);
	}

	Vector2 operator /(float _value)
	{
		assert(_value);

		return Vector2(x / _value, y / _value);
	}

	void operator /=(Vector2 _other)
	{
		if (0 == _other.x && 0 == _other.y)
			assert(nullptr);

		x /= _other.x;
		y /= _other.y;
	}

	void operator /=(int _value)
	{
		assert(_value);

		x /= float(_value);
		y /= (float)_value;
	}

	void operator /=(float _value)
	{
		assert(_value);

		x /= _value;
		y /= _value;
	}

public :
	Vector2() : x(0.f), y(0.f) {}
	~Vector2() {}

	Vector2(int _x, int _y) : x(float(_x)), y(float(_y)) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(POINT _other) : x(float(_other.x)), y(float(_other.y)) {}
};