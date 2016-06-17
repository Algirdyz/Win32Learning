#pragma once
#include <vector>
#include "Rect.h"
#include "MainContainer.h"
#include "Corners.h"


struct Coordinates;

void InsertRectangleIntoGrid(MainContainer* container, int addIndexX, int addIndexY, Rect rectangleToAdd, Corners corner = lowerLeft);
int GetRowIndexToFitRectangle(MainContainer container, int addIndexX, Rect rectangle, Corners corner);
int GetColumnIndexToFitRectangle(MainContainer container, int addIndexY, Rect rectangle, Corners corner);

class RectangleBuilder
{
	
public:
	RectangleBuilder();
	~RectangleBuilder();

	std::vector<Rect> FitRectangles(std::vector<Rect> rectangles, Rect mainRectangle);
};

