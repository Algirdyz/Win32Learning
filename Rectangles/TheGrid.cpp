#include "stdafx.h"
#include "TheGrid.h"
#include "RectangleBuilder.h"


bool TheGrid::CheckGap(Coordinates coordinates, float height, float width, bool cornerUsed)
{
	if (cornerUsed == false)
		return false;
	
	bool a = true;
	int lastRowIndex = GetRowIndexToFitRectangle(coordinates.y, height, coordinates.corners, &a);
	int lastColumnIndex = GetColumnIndexToFitRectangle(coordinates.x, width, coordinates.corners, &a);

	if (lastRowIndex < -1)
		return false;
	if (lastColumnIndex < -1)
		return false;
	
	int lowerRowIndex = coordinates.y;
	int higherRowIndex = lastRowIndex;
	if(lowerRowIndex > higherRowIndex)
	{
		lowerRowIndex = lastRowIndex;
		higherRowIndex = coordinates.y;
	}

	int lowerColumnIndex = coordinates.x;
	int higherColumnIndex = lastColumnIndex;
	if (lowerColumnIndex > higherColumnIndex)
	{
		lowerColumnIndex = lastColumnIndex;
		higherColumnIndex = coordinates.x;
	}

	for(int i = lowerRowIndex; i <= higherRowIndex; i++)
	{
		for(int j = lowerColumnIndex; j <= higherColumnIndex; j++)
		{
			if (sections[i][j].IsFilled)
				return false;
		}
	}
	return true;
}

TheGrid::TheGrid(float height, float width) : gridHeight(height), gridWidth(width)
{
	std::vector<ContainerSection> firstRow;
	firstRow.push_back(ContainerSection(height, width));
	sections.push_back(firstRow);
}

TheGrid::~TheGrid()
{
}

ContainerSection TheGrid::GetSectionAt(int x, int y)
{
	return sections[y][x];
}

std::vector<ContainerSection> TheGrid::GetRowAt(int index)
{
	return sections[index];
}

int TheGrid::GetNumberOfRows()
{
	return sections.size();
}

int TheGrid::GetNumberOfColumns()
{
	return sections[0].size();
}

void TheGrid::SplitRow(int y, float heightFromTop)
{
	// Calculate height of the new row
	float newRowHeight = sections[y][0].sizeY - heightFromTop;

	// Change existing row size
	for (int index = 0; index < sections[y].size(); index++)
	{
		sections[y][index].sizeY = heightFromTop;
	}
	// Build new row
	std::vector<ContainerSection> newRow;
	newRow.reserve(sections[y].size());
	
	for (int i = 0; i < sections[y].size(); i++)
	{
		ContainerSection newSection = ContainerSection(newRowHeight, sections[y][i].sizeX);
		newSection.IsFilled = sections[y][i].IsFilled;
		newRow.push_back(newSection);
	}

	// Insert new row at index
	sections.insert(sections.begin() + y + 1, newRow);
}

void TheGrid::SplitColumn(int x, float widthFromLeft)
{
	// Calculate width of the new column
	float newColumnWidth = sections[0][x].sizeX - widthFromLeft;

	for (int index = 0; index < sections.size(); index++)
	{
		// Change the width of existing row element
		sections[index][x].sizeX = widthFromLeft;

		// Add a new row element
		ContainerSection newSection = ContainerSection(sections[index][x].sizeY, newColumnWidth);
		newSection.IsFilled = sections[index][x].IsFilled;
		sections[index].insert(sections[index].begin() + x + 1, newSection);
	}
}

bool TheGrid::CheckGridSize(int x, int y)
{
	if (sections.size() <= y)
		return false;
	if (sections[y].size() <= x)
		return false;
	return true;
}

float TheGrid::GetRowHeight(int index)
{
	return sections[index][0].sizeY;
}

float TheGrid::GetRowHeight(int index, int endIndex)
{
	if (endIndex < index)
		return 0;
	float height = 0;
	for (int i = index; i <= endIndex; i++)
	{
		height += sections[i][0].sizeY;
	}
	return height;
}

float TheGrid::GetColumnWidth(int index)
{
	return sections[0][index].sizeX;
}

float TheGrid::GetColumnWidth(int index, int endIndex)
{
	if (endIndex < index)
		return 0;
	float width = 0;
	for (int i = index; i <= endIndex; i++)
	{
		width += sections[0][i].sizeX;
	}
	return width;
}

void TheGrid::FillSections(int startX, int endX, int startY, int endY)
{
	for(int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			sections[i][j].IsFilled = true;
		}
	}
}

// Returns index of last row to fit. If it fit evenly returns -1. If it does not fit, returns -2.
int TheGrid::GetRowIndexToFitRectangle(int addIndexY, float height, Corners corner, bool* wasRowSplit)
{
	float totalHeight = 0;
	if (corner == upperLeft || corner == upperRight)
	{
		for (int i = addIndexY; i < GetNumberOfRows(); i++)
		{
			totalHeight += GetRowHeight(i);

			if (totalHeight == height)
				*wasRowSplit = true;

			if (totalHeight > height)
				return i;
		}
	}
	else
	{
		for (int i = addIndexY; i >= 0; i--)
		{
			totalHeight += GetRowHeight(i);

			if (totalHeight == height)
				*wasRowSplit = true;

			if (totalHeight > height)
				return i;
		}
	}

	return -2;
}

int TheGrid::GetColumnIndexToFitRectangle(int addIndexX, float width, Corners corner, bool* wasColumnSplit)
{
	float totalWidth = 0;
	if (corner == upperLeft || corner == lowerLeft)
	{
		for (int i = addIndexX; i < GetNumberOfColumns(); i++)
		{
			totalWidth += GetColumnWidth(i);

			if (totalWidth == width)
				*wasColumnSplit = true;

			if (totalWidth >= width)
				return i;
		}
	}
	else
	{
		for (int i = addIndexX; i >= 0; i--)
		{
			totalWidth += GetColumnWidth(i);

			if (totalWidth == width)
				*wasColumnSplit = true;

			if (totalWidth >= width)
				return i;
		}
	}

	return -2;
}

std::vector<Coordinates> TheGrid::FindGaps(float height, float width)
{
	std::vector<Coordinates> result;
	for (int index = 0; index < sections.size(); index++)
	{
		for (int innerIndex = 0; innerIndex < sections[index].size(); innerIndex++)
		{
			// Ignore filled sections
			if (sections[index][innerIndex].IsFilled)
				continue;

			// We are looking for a corner. It has to have two neighboring filled sections or edges of the grid
			bool topEdgeFilled = index == 0 || sections[index - 1][innerIndex].IsFilled;
			bool bottomEdgeFilled = index + 1 == GetNumberOfRows() || sections[index + 1][innerIndex].IsFilled;
			bool leftEdgeFilled = innerIndex == 0 || sections[index][innerIndex - 1].IsFilled;
			bool rightEdgeFilled = innerIndex + 1 == GetNumberOfColumns() || sections[index][innerIndex + 1].IsFilled;
			
			bool upperRight = topEdgeFilled && rightEdgeFilled;
			bool lowerRight = bottomEdgeFilled && rightEdgeFilled;
			bool upperLeft = topEdgeFilled && leftEdgeFilled;
			bool lowerLeft = bottomEdgeFilled && leftEdgeFilled;

			if (!(upperRight || lowerRight || upperLeft || lowerLeft))
				continue;

			auto coords = Coordinates{ innerIndex, index, Corners::upperLeft };
			if(CheckGap(coords, height, width, upperLeft))
				result.push_back(coords);
			
			coords.corners = Corners::upperRight;
			if (CheckGap(coords, height, width, upperRight))
				result.push_back(coords);
					
			coords.corners = Corners::lowerLeft;
			if (CheckGap(coords, height, width, lowerLeft))
				result.push_back(coords);

			coords.corners = Corners::lowerRight;
			if (CheckGap(coords, height, width, lowerRight))
				result.push_back(coords);
		}
	}
	return result;
}
