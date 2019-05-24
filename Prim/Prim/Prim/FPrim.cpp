#include "FPrim.h"
#include <iostream>
#include <queue>
#include <filesystem>

FPrim::FPrim(std::vector<std::vector<int>> & instanceData)
{
	loadInstance(instanceData);
}

FPrim::~FPrim()
{
	//none of this is actually necessary
	terminals.clear();
	for (auto & e : graph) {
		e.clear();
	}
	graph.clear();
}

void FPrim::loadInstance(std::vector<std::vector<int>> & instanceData)
{
	vertexCount = instanceData[0][0];
	edgeCount = instanceData[0][1];

	graph.resize(vertexCount + 1);

	int i = 1;
	for (; i < instanceData.size(); i++) {
		if (instanceData[i].size() == 3) {
			graph[instanceData[i][0]].push_back(
				std::make_pair(instanceData[i][1], instanceData[i][2])
			);
			graph[instanceData[i][1]].push_back(
				std::make_pair(instanceData[i][0], instanceData[i][2])
			);
		}
		else if (instanceData[i].size() == 1) {
			terminalCount = instanceData[i][0];
			i++;
			break;
		}
		else {
			std::cout << "\nInvalid instance!\n";
			break;
		}
	}
	for (; i < instanceData.size(); i++) {
		for (auto t : instanceData[i]) {
			terminals.push_back(t);
		}
	}
}

std::vector<std::vector<iPair>> FPrim::run(int origin, float alpha)
{
	auto StartTime = std::chrono::high_resolution_clock::now();
	FHeap priority_queue;
	std::vector<int> key(vertexCount + 1, INT32_MAX);
	std::vector<int> parent(vertexCount + 1, -1);
	std::vector<bool> inMST(vertexCount, false);
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
		for (int i = 0; i < graph[u].size(); i++)
		{
			int v = graph[u][i].first;
			int weight = graph[u][i].second;
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
	lastResult.clear();
	lastResult.resize(vertexCount + 1);
	lastResultLength = 0;
	for (int i = 1; i < parent.size(); i++) {
		lastResult[i].push_back(std::make_pair(parent[i], key[i]));
		lastResultLength += key[i];
	}
	lastResult[origin].clear();

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	lastRuntime = std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	return lastResult;
}

void FPrim::printLastResult()
{
	for (int i = 0; i < lastResult.size(); i++) {
		if (lastResult[i].size() > 0) {
			std::cout << i << " " << lastResult[i][0].first << " " << lastResult[i][0].second << "\n";
		}
	}
	std::cout << "V count: " << vertexCount << "\t T count: " << terminalCount << "\t Length: "
		<< lastResultLength << "\t Runtime: " << lastRuntime << "\n";
}
