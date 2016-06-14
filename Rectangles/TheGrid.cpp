#include "stdafx.h"
#include "TheGrid.h"


TheGrid::TheGrid(float height, float width)
{
	gridHeight = height;
	gridWidth = width;
}

TheGrid::~TheGrid()
{
}

ContainerSection TheGrid::GetSectionAt(int x, int y)
{
	return sections[x][y];
}

void TheGrid::SplitRow(int y, float heightFromTop)
{
	// Calculate height of the new row
	float newRowHeight = sections[y][0].sizeY - heightFromTop;

	// Change existing row size
	for (ContainerSection loopSection : sections[y])
	{
		loopSection.sizeY = heightFromTop;
	}
	// Build new row
	std::vector<ContainerSection> newRow;
	newRow.reserve(sections[y].size());
	for (int i = 0; i < sections[y].size(), i++;)
	{
		newRow.push_back(ContainerSection(newRowHeight, sections[y][0].sizeX));
	}

	// Insert new row at index
	sections.insert(sections.begin() + y + 1, newRow);
}

void TheGrid::SplitColumn(int x, float widthFromLeft)
{
	// TODO implement?
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

