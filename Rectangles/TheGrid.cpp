#include "stdafx.h"
#include "TheGrid.h"
#include "RectangleBuilder.h"
#include "Utils.h"


bool TheGrid::CheckGap(Coordinates coordinates, float height, float width)
{
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

	bool cornerFound = false;

	bool topEdgeFilled = false;
	bool bottomEdgeFilled = false;
	bool leftEdgeFilled = false;
	bool rightEdgeFilled = false;

	for(int i = lowerRowIndex; i <= higherRowIndex; i++)
	{
		for(int j = lowerColumnIndex; j <= higherColumnIndex; j++)
		{
			// If any sections are filled, this gap is not valid.
			if (sections[i][j].IsFilled)
				return false;

			// Now we find corners.
			if(!cornerFound)
			{
				switch (coordinates.corners)
				{
				case upperLeft:
					if (!topEdgeFilled && i == lowerRowIndex && (i == 0 || sections[i - 1][j].IsFilled))
						topEdgeFilled = true;
					if (!leftEdgeFilled && j == lowerColumnIndex && (j == 0 || sections[i][j - 1].IsFilled))
						leftEdgeFilled = true;
					break;
				case lowerLeft:
					if (!bottomEdgeFilled && i == higherRowIndex && (i + 1 == GetNumberOfRows() || sections[i + 1][j].IsFilled))
						bottomEdgeFilled = true;
					if (!leftEdgeFilled && j == lowerColumnIndex && (j == 0 || sections[i][j - 1].IsFilled))
						leftEdgeFilled = true;
					break;
				case upperRight:
					if (!topEdgeFilled && i == lowerRowIndex && (i == 0 || sections[i - 1][j].IsFilled))
						topEdgeFilled = true;
					if (!rightEdgeFilled && j == higherColumnIndex && (j + 1 == GetNumberOfColumns() || sections[i][j + 1].IsFilled))
						rightEdgeFilled = true;
					break;
				case lowerRight:
					if (!bottomEdgeFilled && i == higherRowIndex && (i + 1 == GetNumberOfRows() || sections[i + 1][j].IsFilled))
						bottomEdgeFilled = true;
					if (!rightEdgeFilled && j == higherColumnIndex && (j + 1 == GetNumberOfColumns() || sections[i][j + 1].IsFilled))
						rightEdgeFilled = true;
					break;
				}

				if ((topEdgeFilled && leftEdgeFilled) || (topEdgeFilled && rightEdgeFilled) || (bottomEdgeFilled && leftEdgeFilled) || (bottomEdgeFilled && rightEdgeFilled))
					cornerFound = true;
			}
		}
	}
	return cornerFound;
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
		newSection.Color = sections[y][i].Color;
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
		newSection.Color = sections[index][x].Color;
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

void TheGrid::FillSections(int startX, int endX, int startY, int endY, D2D1::ColorF color)
{
	for(int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			sections[i][j].IsFilled = true;
			sections[i][j].Color = color;
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

			// Do a separate check for both rotation possibilities
			float usedHeight = height;
			float usedWidth = width;

			for(int rotationIndex = 0; rotationIndex < 2; rotationIndex++)
			{
				// Check check the section for a gap in all directions
				auto coords = Coordinates{ innerIndex, index, Corners::upperLeft, rotationIndex };
				if (CheckGap(coords, usedHeight, usedWidth))
					result.push_back(coords);

				coords.corners = Corners::upperRight;
				if (CheckGap(coords, usedHeight, usedWidth))
					result.push_back(coords);

				coords.corners = Corners::lowerLeft;
				if (CheckGap(coords, usedHeight, usedWidth))
					result.push_back(coords);

				coords.corners = Corners::lowerRight;
				if (CheckGap(coords, usedHeight, usedWidth))
					result.push_back(coords);

				usedHeight = width;
				usedWidth = height;
			}			
		}
	}
	return result;
}
