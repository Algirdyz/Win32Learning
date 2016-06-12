#include "stdafx.h"
#include "MainContainer.h"

#include <random>
#include <ctime>
#include <d2d1.h>
#include "EdgeCombo.h"

float RandomFloat(float a, float b) {
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

MainContainer::MainContainer(int mainHeight, int mainWidth, int amountToGenerate) 
	:mainRectangle(mainHeight, mainWidth, 0)
{
	srand(time(nullptr));
	generatedRectangles.reserve(amountToGenerate);
	for (int i = 0; i < amountToGenerate; i++) {
		generatedRectangles.push_back(GenerateRandomRectangle(i + 1));
	}

	SolveRectangles();
}


MainContainer::~MainContainer()
{
}

Rect MainContainer::GenerateRandomRectangle(int guid)
{
	float height = RandomFloat(5, mainRectangle._height / 2);
	float width = RandomFloat(5, mainRectangle._width / 2);

	return Rect(height, width, guid);
}

void MainContainer::SolveRectangles()
{
	auto listOfCombos = GenerateCombos();
		
}



std::vector<EdgeCombo> MainContainer::GenerateCombos(EdgeCombo combo, int index)
{
	std::vector<EdgeCombo> combos;
	for (int i = index; i < generatedRectangles.size(); i++)
	{
		if(generatedRectangles[i]._width + combo.totalEdge <= mainRectangle._height)
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

		/*if (generatedRectangles[i]._height + combo.totalEdge <= mainRectangle._height)
		{

			combo.totalEdge += generatedRectangles[i]._height;
			combo.edges.insert(std::make_pair(generatedRectangles[i]._guid, height));
			combos.push_back(combo);
			for (EdgeCombo generatedCombo : GenerateCombos(combo, index + 1))
			{
				combos.push_back(generatedCombo);
			}
		}*/
	}

	return combos;
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

		m_pRenderTarget->DrawRectangle(&rectangle, m_pLightSlateGrayBrush);

		totalRectangleMargin += basicMargins + generatedRectangles[i]._width;
		if (highestRecntangleHeigh < generatedRectangles[i]._height)
			highestRecntangleHeigh = generatedRectangles[i]._height;
	}

	// Draw main rectangle.
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		basicMargins,
		basicMargins * 2 + highestRecntangleHeigh,
		basicMargins + mainRectangle._width,
		basicMargins * 2 + highestRecntangleHeigh + mainRectangle._height
	);

	// Draw a filled rectangle.
	m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

	return basicMargins * 2 + highestRecntangleHeigh + mainRectangle._height + basicMargins;
}

void MainContainer::DrawSolvedRectangles(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush, float topMargin, float leftMargin)
{
	for (int i = 0; i < generatedRectangles.size(); i++)
	{
		float left = leftMargin + generatedRectangles[i].leftCoordinates;
		float top = topMargin + generatedRectangles[i].topCoordinates;
		D2D1_RECT_F rectangle = D2D1::RectF(
			left,
			top,
			left + generatedRectangles[i]._width,
			top + generatedRectangles[i]._height
		);

		m_pRenderTarget->DrawRectangle(&rectangle, m_pLightSlateGrayBrush);
	}
}
