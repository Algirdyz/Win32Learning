#include "stdafx.h"
#include "RectangleBuilder.h"
#include "ContainerSection.h"


RectangleBuilder::RectangleBuilder()
{
}


RectangleBuilder::~RectangleBuilder()
{
}

std::vector<Rect> RectangleBuilder::FitRectangles(std::vector<Rect> rectangles, Rect mainRectangle)
{
	std::vector<Rect> result;



	return result;
}

int GetRowIndexToFitRectangle(MainContainer container, int addIndexY, Rect rectangle, Corners corner)
{
	float totalHeight = 0;
	if(corner == upperLeft || corner == upperRight)
	{
		for (int i = addIndexY; i < container.grid.GetNumberOfRows(); i++)
		{
			totalHeight += container.grid.GetRowHeight(i);

			if (totalHeight > rectangle._height)
				return i;

			if (totalHeight == rectangle._height)
				return -1;
		}
	}
	else
	{
		for (int i = addIndexY; i >= 0; i--)
		{
			totalHeight += container.grid.GetRowHeight(i);

			if (totalHeight > rectangle._height)
				return i;

			if (totalHeight == rectangle._height)
				return -1;
		}
	}
	
	return -1;
}

int GetColumnIndexToFitRectangle(MainContainer container, int addIndexX, Rect rectangle, Corners corner)
{
	float totalWidth = 0;
	if(corner == upperLeft || corner == lowerLeft)
	{
		for (int i = addIndexX; i < container.grid.GetNumberOfColumns(); i++)
		{
			totalWidth += container.grid.GetColumnWidth(i);

			if (totalWidth > rectangle._width)
				return i;

			if (totalWidth == rectangle._width)
				return -1;
		}
	}
	else
	{
		for (int i = addIndexX; i >= 0; i--)
		{
			totalWidth += container.grid.GetColumnWidth(i);

			if (totalWidth > rectangle._width)
				return i;

			if (totalWidth == rectangle._width)
				return -1;
		}
	}
	
	return -1;
}


void InsertRectangleIntoGrid(MainContainer* container, int addIndexX, int addIndexY, Rect rectangleToAdd, Corners corner)
{
	// Check if section index is valid
	if (!container->grid.CheckGridSize(addIndexX, addIndexY))
		return;

	ContainerSection section = container->grid.GetSectionAt(addIndexY, addIndexX);

	// Prepare row split			
	int rowToSplit = GetRowIndexToFitRectangle(*container, addIndexY, rectangleToAdd, corner);
	float heightFromTop = rectangleToAdd._height - container->grid.GetRowHeight(addIndexY, rowToSplit - 1);		
		
	// Prepare column split
	int columnToSplit = GetColumnIndexToFitRectangle(*container, addIndexX, rectangleToAdd, corner);
	float widthFromLeft = rectangleToAdd._width - container->grid.GetColumnWidth(addIndexX, columnToSplit - 1);
	
	// Do splits. If index is less than zero than rectangle ends on a line;
	if (rowToSplit >= 0)
		container->grid.SplitRow(rowToSplit, heightFromTop);
	if (columnToSplit >= 0)
		container->grid.SplitColumn(columnToSplit, widthFromLeft);

	container->grid.FillSections(addIndexX, columnToSplit, addIndexY, rowToSplit);
}
