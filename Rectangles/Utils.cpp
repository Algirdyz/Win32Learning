#include "stdafx.h"
#include "Utils.h"
#include <string>
#include <sstream>


float RandomFloat(float a, float b) {
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

bool is_number(std::string line)
{
	std::istringstream iss(line);
	float f;
	iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
						  // Check the entire string was consumed and if either failbit or badbit is set
	return !iss.fail();
}
