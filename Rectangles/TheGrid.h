#pragma once
#include "ContainerSection.h"
#include <vector>

class TheGrid
{
private:
	float gridHeight;
	float gridWidth;

	std::vector<std::vector<ContainerSection>> sections;
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
	float GetColumnWidth(int index);

};

