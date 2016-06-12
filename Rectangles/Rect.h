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
	float topCoordinates = 0;
	float leftCoordinates = 0;
};

