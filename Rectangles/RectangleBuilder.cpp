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
	float heightFromTop = rectangleToAdd._height - container->grid.GetRowHeight(addIndexY, rowToSplit - 1);		
		
	// Prepare column split
	bool *columnNoSplit = new bool;
	*columnNoSplit = false;
	int columnToSplit = container->grid.GetColumnIndexToFitRectangle(addIndexX, rectangleToAdd._width, corner, columnNoSplit);
	float widthFromLeft = rectangleToAdd._width - container->grid.GetColumnWidth(addIndexX, columnToSplit - 1);
	
	// Do splits. If index is less than zero than rectangle ends on a line;
	if (!(*rowNoSplit))
		container->grid.SplitRow(rowToSplit, heightFromTop);
	if (!*columnNoSplit)
		container->grid.SplitColumn(columnToSplit, widthFromLeft);

	delete rowNoSplit;
	delete columnNoSplit;

	container->grid.FillSections(addIndexX, columnToSplit, addIndexY, rowToSplit);
}
