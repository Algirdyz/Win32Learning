#include "stdafx.h"
#include "Rect.h"


Rect::Rect(float height, float width, int guid)
{
	_height = height;
	_width = width;
	_guid = guid;
}


Rect::~Rect()
{
}

float Rect::Area()
{
	return _height * _width;
}
