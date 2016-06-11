#include "stdafx.h"
#include "Utils.h"
#include <string>
#include <sstream>


Utils::Utils()
{
}


Utils::~Utils()
{
}

bool Utils::is_number(std::string line)
{
	std::istringstream iss(line);
	float f;
	iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
						  // Check the entire string was consumed and if either failbit or badbit is set
	return !iss.fail();
}
