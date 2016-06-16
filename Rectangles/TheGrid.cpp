#include "stdafx.h"
#include "TheGrid.h"


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
	return sections[x][y];
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
		newRow.push_back(ContainerSection(newRowHeight, sections[y][i].sizeX));
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
		sections[index].insert(sections[index].begin() + x + 1, ContainerSection(sections[index][x].sizeY, newColumnWidth));
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
