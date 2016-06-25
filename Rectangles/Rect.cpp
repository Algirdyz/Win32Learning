#include "stdafx.h"
#include "Rect.h"
#include "Utils.h"


Rect::Rect(float height, float width, int guid): 
	_color(D2D1::ColorF(RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1), 1)) 
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
