#pragma once
#include "difinitions.h"
#include "binary_heap.h"
#include "FPrim.h"

class FLNS
{
public:
	FLNS(std::vector<std::vector<int>> & instance);
	FLNS(FGraph & graph);
	~FLNS();

	FGraph graph;
	int lastResultLength = INT32_MAX;
	int lastRunTime = INT32_MAX;

	FGraph lastRunGraph;
	FGraph run();
	void printLastResult(bool printGraph = false, std::string instance = " ");
	void FLNS::runBattery(int runs = 10, std::string instance = " ");
	static void expandNeighborhood(FGraph & original, FGraph & toMerge, double destruction);
};
