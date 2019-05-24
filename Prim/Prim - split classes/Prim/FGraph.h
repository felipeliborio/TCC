#pragma once
#include "difinitions.h"

class FGraph
{
public:
	FGraph(std::vector<std::vector<int>> & instance = std::vector<std::vector<int>>());
	~FGraph();

	int vertexCount = INT32_MIN;
	int edgeCount = INT32_MIN;
	int terminalCount = INT32_MIN;
	int length = 0;

	std::vector<int> terminals;
	std::vector<std::vector<iPair>> graph;

	void loadInstance(std::vector<std::vector<int>> & instanceData);
	void addVertex(std::vector<int> & vertex);
	void deleteVertex(std::vector<int> & vertex);
	void printGraph();
};
