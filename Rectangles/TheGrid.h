#pragma once
#include "ContainerSection.h"
#include <vector>
#include "Rect.h"
#include "Coordinates.h"

class TheGrid
{
private:
	float gridHeight;
	float gridWidth;

	std::vector<std::vector<ContainerSection>> sections;

	bool CheckGap(Coordinates coordinates, float height, float width);

public:
	TheGrid(float height, float width);
	~TheGrid();

	ContainerSection GetSectionAt(int x, int y);
	std::vector<ContainerSection> GetRowAt(int index);

	int GetNumberOfRows();
	int GetNumberOfColumns();

	void SplitRow(int y, float heightFromTop);
	void SplitColumn(int x, float widthFromLeft);

	bool CheckGridSize(int x, int y);

	float GetRowHeight(int index);
	float GetRowHeight(int index, int endIndex);

	float GetColumnWidth(int index);
	float GetColumnWidth(int index, int endIndex);

	void FillSections(int startX, int endX, int startY, int endY);

	int GetRowIndexToFitRectangle(int addIndexY, float height, Corners corner, bool* wasRowSplit);

	int GetColumnIndexToFitRectangle(int addIndexX, float width, Corners corner, bool* wasColumnSplit);

	std::vector<Coordinates> FindGaps(float height, float width);


};

