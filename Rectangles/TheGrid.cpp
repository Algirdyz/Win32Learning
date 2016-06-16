#include "stdafx.h"
#include "TheGrid.h"


TheGrid::TheGrid(float height, float width)
{
	gridHeight = height;
	gridWidth = width;

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

	auto a = sections[y].size();

	for (int i = 0; i < a; i++)
	{
		newRow.push_back(ContainerSection(newRowHeight, sections[y][0].sizeX));
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
		sections[index].insert(sections[index].begin() + x + 1, ContainerSection(sections[0][x].sizeY, newColumnWidth));
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

float TheGrid::GetColumnWidth(int index)
{
	return sections[0][index].sizeX;
}

