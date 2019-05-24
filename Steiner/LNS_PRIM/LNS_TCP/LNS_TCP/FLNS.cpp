#include "FLNS.h"

FLNS::FLNS(std::vector<std::vector<int>>& instance)
{
	graph = FGraph(instance);
	graph.simplifyInstance();
}

FLNS::FLNS(FGraph & graph)
{
	this->graph = graph;
	this->graph.simplifyInstance();
}

FLNS::~FLNS()
{
}

FGraph FLNS::run()
{
	auto StartTime = std::chrono::high_resolution_clock::now();

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distrTerm(0, graph.terminalCount - 1); // define the range
	std::uniform_int_distribution<> distrVtx(1, graph.vertexCount);
	FGraph bestSolution = FPrim(this->graph).run(graph.terminals[distrTerm(eng)]);
	FGraph newSolution = bestSolution;
	int sparsity = (int) (100 * (graph.vertexCount / (double) graph.edgeCount));
	int top = 2 * sparsity;
	if (top > 100) {
		top = 100;
	}
	std::uniform_int_distribution<> distrDest(sparsity / 2, top);
	for (int i = 0; i < 2500 + graph.vertexCount; i++) {
		double destruction = distrDest(eng) / 100.0;
		expandNeighborhood(newSolution, graph, destruction);

		double alpha =  (sqrt(graph.vertexCount / (double) graph.terminalCount) / 50) 
						/ 
						(graph.edgeCount / (double) graph.vertexCount);

		newSolution = FPrim(newSolution).run(distrVtx(eng), alpha);

		if (newSolution.length > bestSolution.length) {
			newSolution = bestSolution;
		}
		else {
			bestSolution = newSolution;
		}
	}
	
	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRunTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	lastRunGraph = bestSolution;
	lastRunGraph.vertexCount = graph.vertexCount;
	lastRunGraph.terminalCount = graph.terminalCount;
	lastRunGraph.terminals = graph.terminals;
	lastRunGraph.bIsTerminal = graph.bIsTerminal;
	lastRunGraph.simplifyInstance();

	return lastRunGraph;
}

void FLNS::expandNeighborhood(FGraph & original, FGraph & full, double destruction)
{
	int maxNewEdges = (int)(destruction * full.edgeCount);
	int edgesInserted = 0;

	std::vector<bool> added(original.graph.size(), false);
	std::vector<int> toVisit;

	for (int i = 0; i < original.vertexCount; i++) {
		if (!original.graph[i].empty()) {
			toVisit.push_back(i);
			added[i] = true;
		}
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator

	while (edgesInserted < maxNewEdges && !toVisit.empty()) {
		std::uniform_int_distribution<> distr(0, toVisit.size() - 1); // define the range
		int chosenInd = distr(eng);

		int vertex1 = toVisit[chosenInd];
		std::swap(toVisit[chosenInd], toVisit[toVisit.size() - 1]);
		toVisit.pop_back();

		std::vector<iPair> edgesToInsert = full.graph[vertex1];
		
		for (auto & edge : edgesToInsert) {
			std::vector<int> newE = { vertex1, edge.first, edge.second };
			original.addEdge(newE);
			edgesInserted++;

			if (!added[edge.first]) {
				toVisit.push_back(edge.first);
				added[edge.first] = true;
			}
		}
	}
}

void FLNS::printLastResult(bool printGraph, std::string instance)
{
	if (printGraph) {
		std::cout << instance << "\n\ttime: " << lastRunTime << "\t";
		lastRunGraph.printGraph();
	}
	else {
		std::string ph = "instance";
		if (instance.size() > ph.size()) {
			ph.resize(instance.size());
		}
		else {
			instance.resize(ph.size());
		}
		std::cout << ph << "\tv\te\tt\tlen\ttime\n";
		std::cout << instance << "\t" << lastRunGraph.vertexCount << "\t" <<
			lastRunGraph.edgeCount << "\t" << lastRunGraph.terminalCount <<
			"\t" << lastRunGraph.length << "\t" << lastRunTime << "\n\n";
	}
}

template<class T>
double vecAvg(std::vector<T> & vec) 
{
	double out = 0;
	for (auto & e : vec) out += e;
	return out / vec.size();
}

template<class T>
T vecMode(std::vector<T> & vec) 
{
	std::sort(vec.begin(), vec.end());
	int count = 0;
	T curr = vec[0];
	int modeCount = 0;
	T mode = vec[0];
	for (auto & e : vec) {
		if (e == curr) {
			count++;
		}
		else {
			if (count > modeCount) {
				mode = curr;
				modeCount = count;
			}
			curr = e;
			count = 1;
		}
	}
	return mode;
}

template<class T>
double vecMedian(std::vector<T> & vec)
{
	double median;
	std::sort(vec.begin(), vec.end());

	if (vec.size() % 2 == 0) {
		median = (vec[(vec.size() / 2) - 1] + vec[vec.size() / 2]) / 2.0;
	}
	else {
		median = vec[vec.size() / 2];
	}

	return median;
}

void FLNS::runBattery(int runs, std::string instance)
{
	std::vector<int> lengthVec, runTimeVec;
	for (int i = 0; i < runs; i++) {
		run();
		lengthVec.push_back(lastRunGraph.length);
		runTimeVec.push_back(lastRunTime);
	}
	double lengthAvg = vecAvg(lengthVec);
	int lengthMode = vecMode(lengthVec);
	double lengthMedian = vecMedian(lengthVec);
	double timeAvg = vecAvg(runTimeVec);
	int lengthMin = *std::min_element(lengthVec.begin(), lengthVec.end());
	int lengthMax = *std::max_element(lengthVec.begin(), lengthVec.end());

	std::cout << instance << "\t" << lengthMin << "\t" << lengthMax << "\t" <<
			lengthAvg << "\t" << lengthMode << "\t" << lengthMedian << "\t" << timeAvg << "\n";
}
