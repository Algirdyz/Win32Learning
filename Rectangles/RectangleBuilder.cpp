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

// Recursive method loops through different rectangle configurations until it finds the correct one
ReturnCode FitRectangles(MainContainer* container, MainContainer* originalContainer, HWND hwnd, int rectangleIndex)
{
	if (container->generatedRectangles.size() <= rectangleIndex)
	{
		if (container->generatedRectangles.size() == container->addedRectangles.size())
			return Success;

		if (container->generatedRectangles.size() < container->addedRectangles.size())
			throw;

		return NoRectangles;		
	}
	auto rectangle = container->generatedRectangles[rectangleIndex];
	auto remainingGaps = container->grid.FindGaps(rectangle._height, rectangle._width);

	if (remainingGaps.size() == 0)
		return NoGaps;

	int nextRectangleIndex = rectangleIndex + 1;
	
	for(int j = 0; j < remainingGaps.size(); j++)
	{
		auto newContainer = *container;

		InsertRectangleIntoGrid(&newContainer, remainingGaps[j].x, remainingGaps[j].y, rectangle, remainingGaps[j].corners);
		newContainer.addedRectangles.push_back(rectangle);

		*originalContainer = newContainer;

		Rectangles *pDemoApp = reinterpret_cast<Rectangles *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));
		pDemoApp->OnRender();	

		switch (FitRectangles(&newContainer, originalContainer, hwnd, nextRectangleIndex))
		{
		case NoRectangles:
			return NoRectangles;
		case NoGaps:

			break;
		case Success:
			*container = newContainer;
			return Success;
		case Finished:
			return Finished;
		}
	}

	return Finished;
}

ReturnCode SolveRectangles(MainContainer* container, HWND hwnd)
{
	auto result = FitRectangles(container, container, hwnd);

	return result;
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
