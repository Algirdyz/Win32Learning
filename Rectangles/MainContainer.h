#include "Rectangle.h"

#include <vector>

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

	Rect GenerateRandomRectangle();

	void DrawRectangles(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* solidBrush);
};

