#include "FGraph.h"

FGraph::FGraph(std::vector<std::vector<int>> & instanceData)
{
	if (instanceData.size() > 0) {
		loadInstance(instanceData);
	}
}

FGraph::~FGraph()
{
	//none of this is actually necessary
	terminals.clear();
	for (auto & e : graph) {
		e.clear();
	}
	graph.clear();
}

void FGraph::loadInstance(std::vector<std::vector<int>> & instanceData)
{
	if (instanceData[0].size() == 2) {
		vertexCount = instanceData[0][0];
		edgeCount = instanceData[0][1];
		graph.resize(vertexCount + 1);
	}

	int i = 1;
	for (; i < instanceData.size(); i++) {
		if (instanceData[i].size() == 3) {
			addVertex(instanceData[i]);
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

void FGraph::addVertex(std::vector<int> & vertex) 
{
	graph[vertex[0]].push_back(std::make_pair(vertex[1], vertex[2]));
	graph[vertex[1]].push_back(std::make_pair(vertex[0], vertex[2]));
	length += vertex[2];
}

void FGraph::deleteVertex(std::vector<int> & vertex)
{
	for (int i = 0; i < graph[vertex[0]].size(); i++) {
		if (graph[vertex[0]][i].first == vertex[1]) {
			graph[vertex[0]].erase(graph[vertex[0]].begin() + i);
			break;
		}
	}

	for (int i = 0; i < graph[vertex[1]].size(); i++) {
		if (graph[vertex[1]][i].first == vertex[0]) {
			graph[vertex[1]].erase(graph[vertex[1]].begin() + i);
			break;
		}
	}
}

void FGraph::printGraph()
{
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			std::cout << i << " " << graph[i][j].first << " " << graph[i][j].second << "\n";
		}
	}
	std::cout << "V count: " << vertexCount << "\t T count: " << terminalCount << "\t Length: " << length << "\n";
}
