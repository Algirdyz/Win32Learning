#include "stdafx.h"
#include "MainContainer.h"

#include <random>
#include <ctime>

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
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
