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

class FGraph
{
public:
	FGraph(std::vector<std::vector<int>> & instance = std::vector<std::vector<int>>());
	~FGraph();

	int vertexCount = INT32_MIN;
	int edgeCount = INT32_MIN;
	int terminalCount = INT32_MIN;
	int maxL = INT32_MAX;
	int maxR = INT32_MAX;
	int lCount = -1;
	int rCount = -1;
	int length = 0;

	std::vector<int> terminals;
	std::vector<bool> bIsTerminal;
	std::vector<std::vector<iPair>> graph;

	void loadInstance(std::vector<std::vector<int>> & instanceData);
	void addEdge(std::vector<int> & edge);
	void deleteEdge(std::vector<int> & vertex);
	void simplifyInstance();
	void FGraph::updateLength();
	void printGraph();
};
