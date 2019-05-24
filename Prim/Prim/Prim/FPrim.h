#pragma once
#include <vector>
#include "binary_heap.h"

typedef std::pair<int, int> iPair;//integer pair

// This class represents a directed graph using 
// adjacency list representation 
class FPrim
{
public:
	FPrim(std::vector<std::vector<int>> & instance);
	~FPrim();

	int vertexCount = INT32_MIN;
	int edgeCount = INT32_MIN;
	int terminalCount = INT32_MIN;
	int lastResultLength = INT32_MIN;
	int lastRuntime = INT32_MAX;

	std::vector<int> terminals;
	// In a weighted graph, we need to store vertex 
	// and weight pair for every edge 
	std::vector<std::vector<iPair>> graph;
	std::vector<std::vector<iPair>> lastResult;
	void loadInstance(std::vector<std::vector<int>> & instanceData);
	std::vector<std::vector<iPair>> run(int origin, float alpha = 0.0);
	void printLastResult();
};

