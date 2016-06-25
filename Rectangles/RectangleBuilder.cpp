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

		return Deadend;		
	}
	auto rectangle = container->generatedRectangles[rectangleIndex];
	auto remainingGaps = container->grid.FindGaps(rectangle._height, rectangle._width);

	if (remainingGaps.size() == 0)
		return Deadend;

	if (rectangleIndex == 0)
		remainingGaps = std::vector<Coordinates>{ remainingGaps[0], remainingGaps[4] };

	int nextRectangleIndex = rectangleIndex + 1;
	
	auto newContainer = *container;

	for(int j = 0; j < remainingGaps.size(); j++)
	{
		auto loopContainer = newContainer;

		InsertRectangleIntoGrid(&loopContainer, remainingGaps[j], rectangle);
		loopContainer.addedRectangles.push_back(rectangle);

		*originalContainer = loopContainer;

		//Rectangles *pDemoApp = reinterpret_cast<Rectangles *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd,GWLP_USERDATA)));
		//pDemoApp->OnRender();	

		//Sleep(10);

		switch (FitRectangles(&loopContainer, originalContainer, hwnd, nextRectangleIndex))
		{		
		case Success:
			*container = loopContainer;
			return Success;
		case Deadend:

			break;
		}
	}
	return Deadend;
}

ReturnCode SolveRectangles(MainContainer* container, HWND hwnd)
{
	auto result = FitRectangles(container, container, hwnd);

	return result;
}

void InsertRectangleIntoGrid(MainContainer* container, Coordinates coords, Rect rectangleToAdd)
{
	// Check if section index is valid
	if (!container->grid.CheckGridSize(coords.y, coords.x))
		return;

	int height = coords.rotation == 0 ? rectangleToAdd._height : rectangleToAdd._width;
	int width = coords.rotation == 0 ? rectangleToAdd._width : rectangleToAdd._height;

	ContainerSection section = container->grid.GetSectionAt(coords.x, coords.y);

	// Prepare row split
	bool *rowNoSplit = new bool;
	*rowNoSplit = false;
	int rowToSplit = container->grid.GetRowIndexToFitRectangle(coords.y, height, coords.corners, rowNoSplit);
	float heightFromTop;
	if (coords.corners == upperLeft || coords.corners == upperRight)
		heightFromTop = height - container->grid.GetRowHeight(coords.y, rowToSplit - 1);
	else
		heightFromTop = container->grid.GetRowHeight(rowToSplit, coords.y) - height;

	// Prepare column split
	bool *columnNoSplit = new bool;
	*columnNoSplit = false;
	int columnToSplit = container->grid.GetColumnIndexToFitRectangle(coords.x, width, coords.corners, columnNoSplit);
	float widthFromLeft;
	if (coords.corners == upperLeft || coords.corners == lowerLeft)
		widthFromLeft = width - container->grid.GetColumnWidth(coords.x, columnToSplit - 1);
	else
		widthFromLeft = container->grid.GetColumnWidth(columnToSplit, coords.x) - width;
	
	
	// Do splits. We do not split if methods above returned a true boolean. Which means that a rectangle ends on a line.
	if (!(*rowNoSplit))
		container->grid.SplitRow(rowToSplit, heightFromTop);
	if (!*columnNoSplit)
		container->grid.SplitColumn(columnToSplit, widthFromLeft);

	
	// Fill sections based on corner. Pointer to bool is added to increment the row/column index if one was inserted.
	switch (coords.corners)
	{
	case upperLeft:
		container->grid.FillSections(coords.x, columnToSplit, coords.y, rowToSplit, rectangleToAdd._color);
		break;
	case upperRight:
		container->grid.FillSections(columnToSplit + !*columnNoSplit, coords.x + !*columnNoSplit, coords.y, rowToSplit, rectangleToAdd._color);
		break;
	case lowerLeft:
		container->grid.FillSections(coords.x, columnToSplit, rowToSplit + !(*rowNoSplit), coords.y + !(*rowNoSplit), rectangleToAdd._color);
		break;
	case lowerRight:
		container->grid.FillSections(columnToSplit + !*columnNoSplit, coords.x + !*columnNoSplit, rowToSplit + !(*rowNoSplit), coords.y + !(*rowNoSplit), rectangleToAdd._color);
		break;
	}

	delete rowNoSplit;
	delete columnNoSplit;
}
