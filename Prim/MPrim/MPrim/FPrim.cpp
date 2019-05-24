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

FGraph FPrim::run(int origin, double alpha)
{
	auto StartTime = std::chrono::high_resolution_clock::now();

	auto terminalsLeft = graph.terminals;
	FHeap priority_queue;
	std::vector<int> distanceVec(graph.vertexCount + 1, INT32_MAX);
	std::vector<int> parentVec(graph.vertexCount + 1, -1);
	std::vector<bool> inMST(graph.vertexCount + 1, false);
	FVertex pqItem = { origin, 0 };
	heapInsert(priority_queue, pqItem);
	distanceVec[origin] = 0;

	while (priority_queue.end != -1)
	{
		int alphaK = (int)(alpha * priority_queue.end);
		int toExtract = alphaK;
		if (alphaK > 1) {
			toExtract = rand() % alphaK;
		}
		int vertex1 = heapExtract(priority_queue, toExtract).vertex;
		inMST[vertex1] = true;
		
		for (int i = 0; i < graph.graph[vertex1].size(); i++)
		{
			int vertex2 = graph.graph[vertex1][i].first;
			int distance = graph.graph[vertex1][i].second;
			if (!inMST[vertex2] && distanceVec[vertex2] > distance)
			{
				distanceVec[vertex2] = distance;
				pqItem = { vertex2, distance };
				heapInsert(priority_queue, pqItem);
				parentVec[vertex2] = vertex1;
			}
		}
	}

	std::vector<std::vector<iPair>> lastResult;
	lastResult.resize(graph.vertexCount + 1);
	lastResultLength = 0;



	for (int i = 1; i < parentVec.size(); i++) {
		if (parentVec[i] != -1) {
			lastResult[i].push_back(std::make_pair(parentVec[i], distanceVec[i]));
			lastResult[parentVec[i]].push_back(std::make_pair(i, distanceVec[i]));
			lastResultLength += distanceVec[i];
		}
	}


	lastResult[origin].clear();

	lastRunGraph.graph = lastResult;
	lastRunGraph.vertexCount = graph.vertexCount;
	lastRunGraph.terminalCount = graph.terminalCount;
	lastRunGraph.terminals = graph.terminals;
	lastRunGraph.bIsTerminal = graph.bIsTerminal;

	lastRunGraph.simplifyInstance();

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRunTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	return lastRunGraph;
}

void FPrim::printLastResult(bool printGraph)
{
	if (printGraph) {
		lastRunGraph.printGraph();
		std::cout << "Runtime: " << lastRunTime << "\n";
	}
	else {
		std::cout << "V: " << lastRunGraph.vertexCount << "\tE: " <<
			lastRunGraph.edgeCount << "\tT: " <<
			lastRunGraph.terminalCount << "\tLength:  " << lastRunGraph.length <<
			"\tRuntime: " << lastRunTime << "\n";
	}
}
