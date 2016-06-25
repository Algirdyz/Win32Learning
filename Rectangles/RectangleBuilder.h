#pragma once
#include <vector>
#include "Rect.h"
#include "MainContainer.h"
#include "Corners.h"
#include "ReturnCodeEnum.h"


struct Coordinates;

void InsertRectangleIntoGrid(MainContainer* container, int addIndexX, int addIndexY, Rect rectangleToAdd, Corners corner = lowerLeft);
ReturnCode FitRectangles(MainContainer* container, MainContainer* originalContainer, HWND hwnd, int rectangleIndex = 0);
ReturnCode SolveRectangles(MainContainer* container, HWND hwnd);

class RectangleBuilder
{
	
public:
	RectangleBuilder();
	~RectangleBuilder();

	
};

