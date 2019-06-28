#pragma once
#include "definitions.h"
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
	std::chrono::time_point<std::chrono::steady_clock> lastStartTime;
	long lastRunTime = INT32_MAX;
	long lastRunTimeToBest = INT32_MAX;

	FGraph lastRunGraph;
	FGraph run(int limit = 500);
	void printLastResult(bool printGraph = false, std::string instance = " ");
	void runBattery(int runs = 10, std::string instance = " ", int limit = 500);
	static void expandNeighborhood(FGraph & original, FGraph & toMerge, double destruction);

private:
	void runMainLoop(FGraph& bestSolution, FGraph& newSolution,
		std::mt19937& eng, uni_dist& distrDest, uni_dist& distrVtx,
		int max_phase_1 = 5000, int max_phase_2 = 1250, bool LRFocus = false);
};
