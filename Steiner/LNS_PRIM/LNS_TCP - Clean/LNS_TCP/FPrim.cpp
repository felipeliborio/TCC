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
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator

	auto StartTime = std::chrono::high_resolution_clock::now();

	origin = checkOrigin(origin);//in case it is a vertex of degree 0

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
		int toExtract = 0;
		int alphaK = (int)(alpha * priority_queue.end);
		if (alphaK > 1) {
			toExtract = eng() % alphaK;
		}

		int vertex1 = heapExtract(priority_queue, toExtract).vertex;
		inMST[vertex1] = true;

		if (graph.bIsTerminal[vertex1]) {
			auto del = std::find(terminalsLeft.begin(), terminalsLeft.end(), vertex1);
			if (del != terminalsLeft.end()) {
				terminalsLeft.erase(del);
			}
		}

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

	lastRunGraph = buildGraph(parentVec, distanceVec);
	lastRunGraph.simplifyInstance();

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRunTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();
	
	return lastRunGraph;
}

FGraph FPrim::buildGraph(std::vector<int>& parentVec, std::vector<int>& distanceVec)
{
	FGraph output;
	std::vector<std::vector<iPair>> adj;

	adj.resize(graph.vertexCount + 1);
	lastResultLength = 0;

	for (int i = 1; i < parentVec.size(); i++) {
		if (parentVec[i] != -1) {
			adj[i].push_back(std::make_pair(parentVec[i], distanceVec[i]));
			adj[parentVec[i]].push_back(std::make_pair(i, distanceVec[i]));
			lastResultLength += distanceVec[i];
		}
	}

	output.graph = adj;
	output.vertexCount = graph.vertexCount;
	output.terminalCount = graph.terminalCount;
	output.terminals = graph.terminals;
	output.bIsTerminal = graph.bIsTerminal;
	output.maxL = graph.maxL;
	output.maxR = graph.maxR;
	return output;
}

int FPrim::checkOrigin(int origin)
{
	int aux = origin;
	while (graph.graph[aux].empty()) {
		aux++;
		if (aux > graph.vertexCount) {
			aux = 0;
		}
		else if (aux == origin) {
			break;
		}
	}
	return aux;
}

void FPrim::printLastResult(bool printGraph, std::string instance)
{
	if (printGraph) {
		std::cout << instance << "\n\ttime: " << lastRunTime << "\t";
		lastRunGraph.printGraph();
	}
	else {
		std::cout << "instance\tv\te\tt\tlen\ttime\n";
		std::cout << instance << "\t" << lastRunGraph.vertexCount << "\t" <<
			lastRunGraph.edgeCount << "\t" << lastRunGraph.terminalCount <<
			"\t" << lastRunGraph.length << "\t" << lastRunTime << "\n";
	}
}