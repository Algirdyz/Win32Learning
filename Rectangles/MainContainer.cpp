#include "stdafx.h"
#include "MainContainer.h"

#include <random>
#include <ctime>
#include <d2d1.h>

float RandomFloat(float a, float b) {
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

MainContainer::MainContainer(int mainHeight, int mainWidth, int amountToGenerate) 
	:mainRectangle(mainHeight, mainWidth)
{
	srand(time(nullptr));
	generatedRectangles.reserve(amountToGenerate);
	for (int i = 0; i < amountToGenerate; i++) {
		generatedRectangles.push_back(GenerateRandomRectangle());
	}
}


MainContainer::~MainContainer()
{
}

Rect MainContainer::GenerateRandomRectangle()
{
	float height = RandomFloat(5, mainRectangle._height / 2);
	float width = RandomFloat(5, mainRectangle._width / 2);

	return Rect(height, width);
}

void MainContainer::DrawRectangles(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush)
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
}
