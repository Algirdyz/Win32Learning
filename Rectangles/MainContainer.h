#include "Rect.h"
#include <vector>
#include "EdgeCombo.h"
#include "TheGrid.h"
#pragma once

struct ID2D1SolidColorBrush;
struct ID2D1HwndRenderTarget;

class MainContainer
{


public:
	MainContainer(int mainHeight, int mainWidth, int amountToGenerate);
	~MainContainer();
	float containerHeight;
	float containerWidth;
	float basicMargins = 10;

	std::vector<Rect> generatedRectangles;
	TheGrid grid;

	Rect GenerateRandomRectangle(int guid);
	
	std::vector<EdgeCombo> GenerateCombos(EdgeCombo combo = EdgeCombo(), int index = 0);

	Rect GetLargestRectangle(std::vector<Rect> rectangles);
	float DrawRectangles(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* solidBrush);
	void DrawSolvedRectangles(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush, float topMargin, float leftMargin);
	void DrawFilledSections(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush, float topMargin, float leftMargin);
};

