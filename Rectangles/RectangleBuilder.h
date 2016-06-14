#pragma once
#include <vector>
#include "Rect.h"
#include "ContainerSection.h"
#include "MainContainer.h"

class RectangleBuilder
{
public:
	RectangleBuilder();
	~RectangleBuilder();

	std::vector<Rect> FitRectangles(std::vector<Rect> rectangles, Rect mainRectangle);

	void RectangleBuilder::GenerateSections(MainContainer container, int addIndexX, int addIndexY, Rect rectangleToAdd);
};

