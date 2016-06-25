#pragma once
#include <d2d1helper.h>

class Rect
{
public:
	Rect(float height, float width, int guid);
	~Rect();

	int _guid;

	float _height;
	float _width;

	bool _rotated = false;

	D2D1::ColorF _color;

	bool operator>(const Rect&) const;
	bool operator<(const Rect&) const;

	float Area() const;
};

