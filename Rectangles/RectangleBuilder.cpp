#include "stdafx.h"
#include "RectangleBuilder.h"
#include "ContainerSection.h"
#include "Rectangles.h"


RectangleBuilder::RectangleBuilder()
{
}


RectangleBuilder::~RectangleBuilder()
{
}

void FitRectangles(MainContainer* container, HWND hwnd)
{
	for (Rect rectangle : container->generatedRectangles)
	{
		auto gaps = container->grid.FindGaps(rectangle._height, rectangle._width);
		
		if(gaps.size() > 0)
			InsertRectangleIntoGrid(container, gaps[0].x, gaps[0].y, rectangle, gaps[0].corners);
		
		Rectangles *pDemoApp = reinterpret_cast<Rectangles *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));
		pDemoApp->OnRender();
	}
}

void InsertRectangleIntoGrid(MainContainer* container, int addIndexX, int addIndexY, Rect rectangleToAdd, Corners corner)
{
	// Check if section index is valid
	if (!container->grid.CheckGridSize(addIndexY, addIndexX))
		return;

	ContainerSection section = container->grid.GetSectionAt(addIndexX, addIndexY);

	// Prepare row split
	bool *rowNoSplit = new bool;
	*rowNoSplit = false;
	int rowToSplit = container->grid.GetRowIndexToFitRectangle(addIndexY, rectangleToAdd._height, corner, rowNoSplit);
	float heightFromTop;
	if (corner == upperLeft || corner == upperRight)
		heightFromTop = rectangleToAdd._height - container->grid.GetRowHeight(addIndexY, rowToSplit - 1);
	else
		heightFromTop = container->grid.GetRowHeight(rowToSplit, addIndexY) - rectangleToAdd._height;

	// Prepare column split
	bool *columnNoSplit = new bool;
	*columnNoSplit = false;
	int columnToSplit = container->grid.GetColumnIndexToFitRectangle(addIndexX, rectangleToAdd._width, corner, columnNoSplit);
	float widthFromLeft;
	if (corner == upperLeft || corner == lowerLeft)
		widthFromLeft = rectangleToAdd._width - container->grid.GetColumnWidth(addIndexX, columnToSplit - 1);
	else
		widthFromLeft = container->grid.GetColumnWidth(columnToSplit, addIndexX) - rectangleToAdd._width;
	
	
	// Do splits. We do not split if methods above returned a true boolean. Which means that a rectangle ends on a line.
	if (!(*rowNoSplit))
		container->grid.SplitRow(rowToSplit, heightFromTop);
	if (!*columnNoSplit)
		container->grid.SplitColumn(columnToSplit, widthFromLeft);

	delete rowNoSplit;
	delete columnNoSplit;

	// Fill sections based on corner
	switch (corner)
	{
	case upperLeft:
		container->grid.FillSections(addIndexX, columnToSplit, addIndexY, rowToSplit);
		break;
	case upperRight:
		container->grid.FillSections(columnToSplit + 1, addIndexX + 1, addIndexY, rowToSplit);
		break;
	case lowerLeft:
		container->grid.FillSections(addIndexX, columnToSplit, rowToSplit + 1, addIndexY + 1);
		break;
	case lowerRight:
		container->grid.FillSections(columnToSplit + 1, addIndexX + 1, rowToSplit + 1, addIndexY + 1);
		break;
	}
}
