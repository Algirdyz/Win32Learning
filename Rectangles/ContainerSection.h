#pragma once
#include <d2d1helper.h>


class ContainerSection
{
public:
	ContainerSection::ContainerSection(float height, float width);
	~ContainerSection();

	float sizeY;
	float sizeX;

	bool IsFilled = false;

	D2D1::ColorF Color = D2D1::ColorF::CornflowerBlue;
};

