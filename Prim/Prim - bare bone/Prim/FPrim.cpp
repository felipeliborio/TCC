#include "FPrim.h"
#include <iostream>
#include <queue>

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
			i++;
			terminalCount = instanceData[i][0];
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

std::vector<FHeap> FPrim::run(int origin)
{
	// Create a priority queue to store vertices that 
	// are being preinMST. This is weird syntax in C++. 
	// Refer below link for details of this syntax 
	std::priority_queue< iPair, std::vector<iPair>, std::greater<iPair> > pq;

	// Create a vector for keys and initialize all 
	// keys as infinite (INF) 
	std::vector<int> key(vertexCount + 1, INT32_MAX);

	// To store parent array which in turn store MST 
	std::vector<int> parent(vertexCount + 1, -1);

	// To keep track of vertices included in MST 
	std::vector<bool> inMST(vertexCount, false);

	// Insert source itself in priority queue and initialize 
	// its key as 0. 
	pq.push(std::make_pair(0, origin));
	key[origin] = 0;

	/* Looping till priority queue becomes empty */
	while (!pq.empty())
	{
		// The first vertex in pair is the minimum key 
		// vertex, extract it from priority queue. 
		// vertex label is stored in second of pair (it 
		// has to be done this way to keep the vertices 
		// sorted key (key must be first item 
		// in pair) 
		int u = pq.top().second;
		pq.pop();

		inMST[u] = true;  // Include vertex in MST 

		// 'i' is used to get all adjacent vertices of a vertex 
		for (int i = 0; i < graph[u].size(); i++)
		{
			// Get vertex label and weight of current adjacent 
			// of u. 
			int v = graph[u][i].first;
			int weight = graph[u][i].second;

			//  If v is not in MST and weight of (u,v) is smaller 
			// than current key of v 
			if (inMST[v] == false && key[v] > weight)
			{
				// Updating key of v 
				key[v] = weight;
				pq.push(std::make_pair(key[v], v));
				parent[v] = u;
			}
		}
	}
	
	int sum = 0;
	// Print edges of MST using parent array 
	for (int i = 1; i < parent.size(); i++) {
		std::cout << i << " " << parent[i] << "\n";
		sum += key[i];
	}
	std::cout << sum << "\n";
	
	return std::vector<FHeap>();
}
