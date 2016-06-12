#include "Rect.h"

#include <vector>
#include "EdgeCombo.h"

struct ID2D1SolidColorBrush;
struct ID2D1HwndRenderTarget;

class MainContainer
{


public:
	MainContainer(int mainHeight, int mainWidth, int amountToGenerate);
	~MainContainer();
	Rect mainRectangle;
	float basicMargins = 10;

	std::vector<Rect> generatedRectangles;

	Rect GenerateRandomRectangle(int guid);

	void SolveRectangles();
	std::vector<EdgeCombo> GenerateCombos(EdgeCombo combo = EdgeCombo(), int index = 0);
	
	float DrawRectangles(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* solidBrush);
	void DrawSolvedRectangles(ID2D1HwndRenderTarget* m_pRenderTarget, ID2D1SolidColorBrush* m_pLightSlateGrayBrush, float topMargin, float leftMargin);
};

