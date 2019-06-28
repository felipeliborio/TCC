#include "FPrim.h"

const int TA = 1;
const int TB = 2;
const int TC = 3;
const int TD = 4;

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

FGraph FPrim::run(int origin, double alpha, bool classify, bool invalidateOverLimit)
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
	//different types of vertices, will store their position in the priority_queue, declare here for performance reasons
	std::vector<int> ta, tb, tc, td;
	std::vector<int> degreeVec;//to keep the degree of each vertex in the tree, must be rebuilt each cicle
	int lCount = 0, rCount = 0;
	std::pair<int, int> lrCount;

	while (priority_queue.end != -1)
	{
		int toExtract = 0;
		if (classify) {
			ta.clear(); tb.clear(); tc.clear(); td.clear();
			
			degreeVec.clear();
			degreeVec.resize(graph.vertexCount + 1, 0);
			
			for (int i = 1; i < parentVec.size(); i++) {
				if (parentVec[i] != -1 && inMST[parentVec[i]]) {
					degreeVec[parentVec[i]]++;
				}
			}
			//this is above the final number, but it would to too slow to discard the leaves before counting
			lrCount = countLAndR(degreeVec);
			lCount = lrCount.first;
			rCount = lrCount.second;
			toExtract = pickNextVertexFromHeap(alpha, eng, lCount, rCount, ta, tb, tc, td, priority_queue, parentVec, degreeVec);
		}
		else {
			int alphaK = (int)(alpha * priority_queue.end);
			if (alphaK > 1) {
				toExtract = eng() % alphaK;
			}
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

	degreeVec.clear();
	degreeVec.resize(graph.vertexCount + 1, 0);

	for (int i = 1; i < lastRunGraph.graph.size(); i++) {
			degreeVec[i] = lastRunGraph.graph[i].size();
	}

	lrCount = countLAndR(degreeVec);
	lCount = lrCount.first;
	rCount = lrCount.second;

	lastRunGraph.lCount = lCount;
	lastRunGraph.rCount = rCount;

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRunTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();
	
	if (invalidateOverLimit && (terminalsLeft.size() > 0 || lCount > graph.maxL || rCount > graph.maxR)) {
		lastRunGraph.length = INT32_MAX;
		lastRunTime = INT32_MAX;
		return lastRunGraph;
	}
	
	return lastRunGraph;
}

std::pair<int, int> FPrim::countLAndR(std::vector<int> & degreeVec)
{
	int lCount = 0, rCount = 0;
	for (int i = 1; i < degreeVec.size(); i++) {
		if (!graph.bIsTerminal[i]) {
			if (degreeVec[i] == 2) {
				lCount++;
			}
			else if (degreeVec[i] > 2) {
				rCount++;
			}
		}
	}
	return std::make_pair(lCount, rCount);
}

int FPrim::pickNextVertexFromHeap(double alpha, std::mt19937& eng, int lCount, int rCount, std::vector<int>& ta,
	std::vector<int>& tb, std::vector<int>& tc, std::vector<int>& td,
	FHeap& priority_queue, std::vector<int>& parentVec, std::vector<int>& degreeVec)
{
	int alphaK = (int)(alpha * priority_queue.end);
	int toExtract = 0;
	for (int i = 0; i <= alphaK; i++) {
		switch (classifyVertex(parentVec[priority_queue.elements[i].vertex], degreeVec)) {//TODO check this (access violation)
		case TA:
			ta.push_back(i);
			break;
		case TB:
			tb.push_back(i);
			break;
		case TC:
			tc.push_back(i);
			break;
		case TD:
			td.push_back(i);
		}
	}

	int txRI = eng();//type "x" random indexer
	if (ta.size() > 0) {
		toExtract = ta[txRI % ta.size()];
	}
	else if (tb.size() > 0) {
		toExtract = tb[txRI % tb.size()];
	}
	else if (graph.maxR - rCount < graph.maxL - lCount) {
		if (tc.size() > 0) {
			toExtract = tc[txRI % tc.size()];
		}
		else {
			goto empty_tx;
		}
	}
	else if (graph.maxL - lCount < graph.maxR - rCount) {
		if (td.size() > 0) {
			toExtract = td[txRI % td.size()];
		}
		else {
			goto empty_tx;
		}
	}
	else {
	empty_tx:
		if (alphaK > 1) {
			toExtract = eng() % alphaK;
		}
	}
	return toExtract;
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

int FPrim::classifyVertex(int vertex, std::vector<int> & degreeVec)
{
	if (vertex == -1) {
		return 0;
	}
	else if (graph.bIsTerminal[vertex]) {
		return TA;
	}
	else if (degreeVec[vertex] > 2) {
		return TB;
	}
	else if (degreeVec[vertex] == 2) {
		return TC;
	}
	else if (degreeVec[vertex] == 1) {
		return TD;
	}
	else {
		return 0;
	}
}

