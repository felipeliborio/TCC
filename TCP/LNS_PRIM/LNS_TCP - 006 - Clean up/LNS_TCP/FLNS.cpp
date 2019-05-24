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

void FLNS::runMainLoop(FGraph& bestSolution, FGraph& newSolution, 
	std::mt19937& eng, uni_dist& distrDest, uni_dist& distrVtx,
	int runs, bool LRFocus)
{
	for (int i = 0; i < runs; i++) {
		double destruction = distrDest(eng) / 100.0;
		expandNeighborhood(newSolution, graph, destruction);

		//TODO alpha still isn't ideal, try to formulate a better equation for it
		//if it isn't possible, try to make values adapt to the instance in a different manner
		double alpha = (sqrt(graph.vertexCount / (double)graph.terminalCount) / (sqrt(graph.vertexCount) + sqrt(graph.terminalCount)))
			/
			(graph.edgeCount / (double)graph.vertexCount);
		if (alpha > 1) {
			alpha = 1;
		}

		if (!LRFocus) {
			if (i > runs - 500) {
				newSolution = FPrim(newSolution).run(distrVtx(eng), alpha, true);
			}
			else {
				newSolution = FPrim(newSolution).run(distrVtx(eng), alpha);
			}
		}
		else {
			newSolution = FPrim(newSolution).run(distrVtx(eng), alpha, true, false);
		}

		if (!LRFocus) {
			if (newSolution.length > bestSolution.length) {
				newSolution = bestSolution;
			}
			else {
				bestSolution = newSolution;
			}
		}
		else {
			if (
				(bestSolution.lCount > graph.maxL && newSolution.rCount <= graph.maxR && newSolution.lCount < bestSolution.lCount) ||
				(bestSolution.rCount > graph.maxR && newSolution.lCount <= graph.maxL && newSolution.rCount < bestSolution.rCount) ||
				(newSolution.lCount + newSolution.rCount < bestSolution.lCount + bestSolution.rCount)
				) {
				bestSolution = newSolution;
			}
			else {
				newSolution = bestSolution;
			}
		}	
	}
}

FGraph FLNS::run()
{
	auto StartTime = std::chrono::high_resolution_clock::now();

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	uni_dist distrTerm(0, graph.terminalCount - 1); // define the range
	uni_dist distrVtx(1, graph.vertexCount);
	FPrim bs(this->graph);
	bs.graph.maxL = INT32_MAX;
	bs.graph.maxR = INT32_MAX;
	FGraph bestSolution = bs.run(graph.terminals[distrTerm(eng)]);
	bestSolution.maxL = graph.maxL;
	bestSolution.maxR = graph.maxR;
	FGraph newSolution = bestSolution;
	int sparsity = (int) (100 * (graph.vertexCount / (double) graph.edgeCount));
	int top = 2 * sparsity;
	if (top > 100) {
		top = 100;
	}
	std::uniform_int_distribution<> distrDest(sparsity / 2, top);
	
	runMainLoop(bestSolution, newSolution, eng, distrDest, distrVtx, 2500 + graph.vertexCount);
	if (bestSolution.lCount > graph.maxL || bestSolution.rCount > graph.maxR) {
		runMainLoop(bestSolution, newSolution, eng, distrDest, distrVtx, 2500 + graph.vertexCount, true);
	}

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRunTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	if (bestSolution.lCount > graph.maxL || bestSolution.rCount > graph.maxR) {
		lastRunGraph.length = INT32_MAX;
		return lastRunGraph;
	}

	lastRunGraph = bestSolution;
	lastRunGraph.vertexCount = graph.vertexCount;
	lastRunGraph.terminalCount = graph.terminalCount;
	lastRunGraph.edgeCount = graph.edgeCount;
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
double vecMedian(std::vector<T> & vec)
{
	double median = INT32_MAX;
	std::sort(vec.begin(), vec.end());
	if (vec.size() == 0) {
		return median;
	}
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
	std::vector<int> lengthVec, runTimeVec, lVec, rVec;

	int solved = 0;
	for (int i = 0; i < runs; i++) {
		run();
		if (lastRunGraph.length != INT32_MAX) {
			lengthVec.push_back(lastRunGraph.length);
			runTimeVec.push_back(lastRunTime);
			lVec.push_back(lastRunGraph.lCount);
			rVec.push_back(lastRunGraph.rCount);
			solved++;
		}
	}

	double lengthAvg = -1, lengthMedian = -1, timeAvg = -1;
	int lengthMin = -1, lengthMax = -1;
	int lMin = -1, lMax = -1, rMin = -1, rMax = -1;

	if (lengthVec.size() > 0) {
		lengthAvg = vecAvg(lengthVec);
		lengthMedian = vecMedian(lengthVec);
		timeAvg = vecAvg(runTimeVec);
		lengthMin = *std::min_element(lengthVec.begin(), lengthVec.end());
		lengthMax = *std::max_element(lengthVec.begin(), lengthVec.end());
		lMin = *std::min_element(lVec.begin(), lVec.end());
		lMax = *std::max_element(lVec.begin(), lVec.end());
		rMin = *std::min_element(rVec.begin(), rVec.end());
		rMax = *std::max_element(rVec.begin(), rVec.end());
	}

	std::cout << instance << "\t" << lengthMin << "\t" << lengthMax << "\t" <<
		lengthAvg << "\t" << lengthMedian << "\t" << timeAvg <<
		"\t" << lMin << "\t" << lMax << "\t" << rMin << "\t" << rMax << "\t" << solved << "\n";
}
