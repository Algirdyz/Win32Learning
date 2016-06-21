#pragma once
class Rect
{
public:
	Rect(float height, float width, int guid);
	~Rect();

	int _guid;

	float _height;
	float _width;

	int rotation = 0;

	float Area();
};

