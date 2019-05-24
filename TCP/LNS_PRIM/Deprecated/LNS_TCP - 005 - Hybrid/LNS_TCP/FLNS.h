#pragma once
#include "difinitions.h"
#include "binary_heap.h"
#include "FPrim.h"

typedef std::uniform_int_distribution<>  uni_dist;

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
	void runBattery(int runs = 10, std::string instance = " ");
	static void expandNeighborhood(FGraph & original, FGraph & toMerge, double destruction);

private:
	void runMainLoop(FGraph& bestSolution, FGraph& newSolution,
		std::mt19937& eng, uni_dist& distrDest, uni_dist& distrVtx, 
		int runs, bool LRFocus = false);
};
