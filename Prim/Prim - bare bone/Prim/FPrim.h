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

	int vertexCount;
	int edgeCount;
	int terminalCount;

	std::vector<int> terminals;
	// In a weighted graph, we need to store vertex 
	// and weight pair for every edge 
	std::vector<std::vector<iPair>> graph;
	void loadInstance(std::vector<std::vector<int>> & instanceData);
	std::vector<FHeap> run(int origin);
};

