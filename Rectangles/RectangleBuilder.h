#pragma once
#include <vector>
#include "Rect.h"
#include "MainContainer.h"


void GenerateSections(MainContainer* container, int addIndexX, int addIndexY, Rect rectangleToAdd);
int GetRowIndexToFitRectangle(MainContainer container, int addIndexX, Rect rectangle);
int GetColumnIndexToFitRectangle(MainContainer container, int addIndexY, Rect rectangle);

class RectangleBuilder
{
	
public:
	RectangleBuilder();
	~RectangleBuilder();

	std::vector<Rect> FitRectangles(std::vector<Rect> rectangles, Rect mainRectangle);
};

