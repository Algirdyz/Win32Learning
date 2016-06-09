#include "Rectangle.h"

#include <vector>

class MainContainer
{


public:
	MainContainer(int mainHeight, int mainWidth, int amountToGenerate);
	~MainContainer();
	Rect mainRectangle;
	std::vector<Rect> generatedRectangles;

	Rect GenerateRandomRectangle();
};

