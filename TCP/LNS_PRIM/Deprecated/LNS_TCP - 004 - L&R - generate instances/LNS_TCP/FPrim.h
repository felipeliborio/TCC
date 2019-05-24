/*
MIT License

Copyright (c) <2019> <Felipe Libório>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
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
	FGraph run(int origin = 1, double alpha = 0.0);
	void printLastResult(bool printGraph = false, std::string instance = " ");

private:
	int classifyVertex(int vertex, std::vector<int> & degreeVec);
	std::pair<int, int> countLAndR(std::vector<int> & degreeVec);
	int pickNextVertexFromHeap(double alpha, std::mt19937& eng, int lCount, int rCount, std::vector<int>& ta, 
				std::vector<int>& tb, std::vector<int>& tc, std::vector<int>& td,
				FHeap& priority_queue, std::vector<int>& parentVec, std::vector<int>& degreeVec);
	FGraph buildGraph(std::vector<int>& parentVec, std::vector<int>& distanceVec, int lCount, int rCount);
	int checkOrigin(int origin);
	std::pair<std::vector<int>, std::vector<int>> getVecPairFromGraph(FGraph& graph, int origin);
	void updateInMSTVec(FGraph& graph, std::vector<bool>& inMST);
	void updatePriorityQueue(FHeap& priority_queue, FGraph& graph, std::vector<int>& parentVec, 
								std::vector<int>& distanceVec, std::vector<bool>& inMST);
};
