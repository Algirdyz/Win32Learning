#pragma once
#include <unordered_map>
#include "Edge.h"

class EdgeCombo
{
public:
	EdgeCombo();
	~EdgeCombo();

	float totalEdge = 0;
	std::unordered_map<int, Edge> edges;
};

