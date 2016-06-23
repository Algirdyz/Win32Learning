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

bool Rect::operator>(const Rect &other) const
{
	return Area() > other.Area();
}

bool Rect::operator<(const Rect &other) const
{
	return Area() < other.Area();
}

float Rect::Area() const
{
	return _height * _width;
}
