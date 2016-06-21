#pragma once
#include <vector>
#include "Rect.h"
#include "MainContainer.h"
#include "Corners.h"


struct Coordinates;

void InsertRectangleIntoGrid(MainContainer* container, int addIndexX, int addIndexY, Rect rectangleToAdd, Corners corner = lowerLeft);
void FitRectangles(MainContainer* container, HWND hwnd);

class RectangleBuilder
{
	
public:
	RectangleBuilder();
	~RectangleBuilder();

	
};

