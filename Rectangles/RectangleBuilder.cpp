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

MainContainer GenerateSections(MainContainer container, int addIndexX, int addIndexY, Rect rectangleToAdd)
{
	// Check if section index is valid
	if (!container.grid.CheckGridSize(addIndexX, addIndexY))
		return container;

	ContainerSection section = container.grid.GetSectionAt(addIndexY, addIndexX);

	// Split row
	if(section.sizeY != rectangleToAdd._height)
	{		
		int rowToSplit = addIndexY;
		float heightFromTop;
		if (section.sizeY < rectangleToAdd._height)
		{
			rowToSplit++;
			heightFromTop = rectangleToAdd._height - container.grid.GetRowHeight(rowToSplit - 1);
		}
		else
		{
			heightFromTop = rectangleToAdd._height;
		}

		container.grid.SplitRow(rowToSplit, heightFromTop);
	}
	
	// Split column
	if(section.sizeX != rectangleToAdd._width)
	{
		int columnToSplit = addIndexX;
		float widthFromLeft;
		if(section.sizeX < rectangleToAdd._width)
		{
			columnToSplit++;
			widthFromLeft = rectangleToAdd._width - container.grid.GetColumnWidth(columnToSplit - 1);
		}
		else
		{
			widthFromLeft = rectangleToAdd._width;
		}

		container.grid.SplitColumn(columnToSplit, widthFromLeft);
	}

	// Compare selected section size to rectangle
	if(section.sizeY >= rectangleToAdd._height && section.sizeX >= rectangleToAdd._width)
	{

		
	}

	auto a = 0;

	return container;
}
