#pragma once
#include "difinitions.h"
#include "binary_heap.h"
#include "FGraph.h"

class FPrim
{
public:
	FPrim(std::vector<std::vector<int>> & instance);
	FPrim(FGraph & graph);
	~FPrim();
	FGraph graph;
	int lastResultLength = INT32_MIN;
	int lastRunTime = INT32_MAX;

	FGraph lastRunGraph;
	FGraph run(int origin = 1, float alpha = 0.0);
	void printLastResult(bool printGraph = false);
};

