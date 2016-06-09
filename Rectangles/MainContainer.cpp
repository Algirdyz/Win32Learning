#include "stdafx.h"
#include "MainContainer.h"

#include <random>



MainContainer::MainContainer(int mainHeight, int mainWidth, int amountToGenerate) 
	:mainRectangle(mainHeight, mainWidth)
{
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
	float height = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mainRectangle._height - 1)));
	float width = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mainRectangle._width - 1)));

	return Rect(height, width);
}
