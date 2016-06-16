#pragma once
#include <vector>
#include "Rect.h"
#include "MainContainer.h"


MainContainer GenerateSections(MainContainer container, int addIndexX, int addIndexY, Rect rectangleToAdd);

class RectangleBuilder
{
public:
	RectangleBuilder();
	~RectangleBuilder();

	std::vector<Rect> FitRectangles(std::vector<Rect> rectangles, Rect mainRectangle);

};

