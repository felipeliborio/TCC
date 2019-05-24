#include "FPrim.h"

FPrim::FPrim(std::vector<std::vector<int>> & instanceData)
{
	graph = FGraph(instanceData);
}

FPrim::FPrim(FGraph & graph)
{
	this->graph = graph;
}

FPrim::~FPrim()
{
}

FGraph FPrim::run(int origin, float alpha)
{
	auto StartTime = std::chrono::high_resolution_clock::now();

	FHeap priority_queue;
	std::vector<int> key(graph.vertexCount + 1, INT32_MAX);
	std::vector<int> parent(graph.vertexCount + 1, -1);
	std::vector<bool> inMST(graph.vertexCount + 1, false);
	FVertex pqItem = { origin, 0 };
	heapInsert(priority_queue, pqItem);
	key[origin] = 0;

	while (priority_queue.end != -1)
	{
		int alphaK = (int)(alpha * priority_queue.end);
		int toExtract = alphaK;
		if (alphaK > 1) {
			toExtract = rand() % alphaK;
		}
		
		int u = heapExtract(priority_queue, toExtract).vertex;
		if (priority_queue.end == -1) {
			int a = 0;
		}
		inMST[u] = true;  // Include vertex in MST 
		// 'i' is used to get all adjacent vertices of a vertex 
		for (int i = 0; i < graph.graph[u].size(); i++)
		{
			int v = graph.graph[u][i].first;
			int weight = graph.graph[u][i].second;
			//  If v is not in MST and weight of (u,v) is smaller 
			// than current key of v 
			if (!inMST[v] && key[v] > weight)
			{
				// Updating key of v 
				key[v] = weight;
				pqItem = { v, weight };
				heapInsert(priority_queue, pqItem);
				parent[v] = u;
			}
		}
	}

	std::vector<std::vector<iPair>> lastResult;
	lastResult.resize(graph.vertexCount + 1);
	lastResultLength = 0;
	for (int i = 1; i < parent.size(); i++) {
		lastResult[i].push_back(std::make_pair(parent[i], key[i]));
		lastResultLength += key[i];
	}
	lastResult[origin].clear();

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRunTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	lastRunGraph.graph = lastResult;
	lastRunGraph.length = lastResultLength;
	lastRunGraph.vertexCount = graph.vertexCount;
	lastRunGraph.edgeCount = graph.vertexCount - 1;
	lastRunGraph.terminalCount = graph.terminalCount;
	lastRunGraph.terminals = graph.terminals;

	return lastRunGraph;
}

void FPrim::printLastResult(bool printGraph)
{
	if (printGraph) {
		lastRunGraph.printGraph();
		std::cout << "Runtime: " << lastRunTime << "\n";
	}
	else {
		std::cout << "V count: " << lastRunGraph.vertexCount << "\tT count: " <<
			lastRunGraph.terminalCount << "\tLength:  " << lastRunGraph.length <<
			"\tRuntime: " << lastRunTime << "\n";
	}
}
