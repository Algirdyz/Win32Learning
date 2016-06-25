#include "stdafx.h"
#include "MainContainer.h"

#include <random>
#include <ctime>
#include <d2d1.h>
#include "EdgeCombo.h"
#include "Utils.h"
#include <algorithm>
#include <functional>


MainContainer::MainContainer(int mainHeight, int mainWidth, int amountToGenerate)
	: grid(mainHeight, mainWidth)
{
	// Init main rectangle
	containerHeight = mainHeight;
	containerWidth = mainWidth;
	
	// Generate random rectangles and store in vector
	srand(time(nullptr));
	generatedRectangles.reserve(amountToGenerate);
	for (int i = 0; i < amountToGenerate; i++) {
		generatedRectangles.push_back(GenerateRandomRectangle(i + 1));
	}
	sort(generatedRectangles.begin(), generatedRectangles.end(), std::greater<Rect>());
}


MainContainer::~MainContainer()
{
}

Rect MainContainer::GenerateRandomRectangle(int guid)
{
	float h = RandomFloat(5, containerHeight/2);
	float w = RandomFloat(5, containerWidth/2);

	return Rect(h, w, guid);
}

std::vector<EdgeCombo> MainContainer::GenerateCombos(EdgeCombo combo, int index)
{
	std::vector<EdgeCombo> combos;
	for (int i = index; i < generatedRectangles.size(); i++)
	{
		if(generatedRectangles[i]._width + combo.totalEdge <= containerHeight)
		{
			combo.totalEdge += generatedRectangles[i]._width;
			combo.edges.insert(std::make_pair(generatedRectangles[i]._guid, width));
			
			auto generatedcombos = GenerateCombos(combo, index + 1);

			if(generatedcombos.size() > 0)
			{
				for (EdgeCombo generatedCombo : generatedcombos)
				{
					combos.push_back(generatedCombo);
				}
			}
			else
			{
				combos.push_back(combo);
			}
		}
	}
	return combos;
}

Rect MainContainer::GetLargestRectangle(std::vector<Rect> rectangles)
{
	Rect result = Rect(0,0,0);
	for (Rect rectangle : rectangles)
	{
		if (result.Area() < rectangle.Area())
			result = rectangle;
	}
	return result;
}

float MainContainer::DrawRectangles(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush)
{
	float totalRectangleMargin = basicMargins;

	float highestRecntangleHeigh = 0;
	// Draw random rectangles
	for (int i = 0; i<generatedRectangles.size(); i++)
	{
		D2D1_RECT_F rectangle = D2D1::RectF(
			totalRectangleMargin,
			basicMargins,
			totalRectangleMargin + generatedRectangles[i]._width,
			basicMargins + generatedRectangles[i]._height
		);

		m_pLightSlateGrayBrush->SetColor(generatedRectangles[i]._color);

		m_pRenderTarget->FillRectangle(&rectangle, m_pLightSlateGrayBrush);

		totalRectangleMargin += basicMargins + generatedRectangles[i]._width;
		if (highestRecntangleHeigh < generatedRectangles[i]._height)
			highestRecntangleHeigh = generatedRectangles[i]._height;
	}

	// Draw main rectangle.
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		basicMargins,
		basicMargins * 2 + highestRecntangleHeigh,
		basicMargins + containerWidth,
		basicMargins * 2 + highestRecntangleHeigh + containerHeight
	);

	m_pLightSlateGrayBrush->SetColor(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));

	// Draw a filled rectangle.
	m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

	return basicMargins * 2 + highestRecntangleHeigh + containerHeight + basicMargins;
}

void MainContainer::DrawSolvedRectangles(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush, float topMargin, float leftMargin)
{
	float rowCoordinatesY = 0;
	for (int rowIndex = 0; rowIndex < grid.GetNumberOfRows(); rowIndex++)
	{
		std::vector<ContainerSection> row = grid.GetRowAt(rowIndex);

		float rowCoordinatesX = 0;
		for (ContainerSection section : row)
		{
			float left = leftMargin + rowCoordinatesX;
			float top = topMargin + rowCoordinatesY;
			D2D1_RECT_F rectangle = D2D1::RectF(
				left,
				top,
				left + section.sizeX,
				top + section.sizeY
			);

			rowCoordinatesX += section.sizeX;

			m_pRenderTarget->DrawRectangle(&rectangle, m_pLightSlateGrayBrush);
		}
		rowCoordinatesY += row[0].sizeY;
	}
}

void MainContainer::DrawFilledSections(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* brush, float topMargin, float leftMargin)
{
	float rowCoordinatesY = 0;
	for (int rowIndex = 0; rowIndex < grid.GetNumberOfRows(); rowIndex++)
	{
		std::vector<ContainerSection> row = grid.GetRowAt(rowIndex);

		float rowCoordinatesX = 0;
		for (ContainerSection section : row)
		{
			if (!section.IsFilled)
			{
				rowCoordinatesX += section.sizeX;
				continue;
			}
			float left = leftMargin + rowCoordinatesX;
			float top = topMargin + rowCoordinatesY;
			D2D1_RECT_F rectangle = D2D1::RectF(
				left,
				top,
				left + section.sizeX,
				top + section.sizeY
			);

			rowCoordinatesX += section.sizeX;
			brush->SetColor(section.Color);
			m_pRenderTarget->FillRectangle(&rectangle, brush);
		}
		rowCoordinatesY += row[0].sizeY;
	}
}

void MainContainer::ClearContainer()
{
	addedRectangles.clear();
	grid = TheGrid(containerHeight, containerWidth);
}
