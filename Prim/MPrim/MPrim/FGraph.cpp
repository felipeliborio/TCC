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
		bIsTerminal.resize(vertexCount + 1, false);
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
		for (auto & t : instanceData[i]) {
			terminals.push_back(t);
			bIsTerminal[t] = true;
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

void FGraph::simplifyInstance()
{
	bool bDone = false;
	while (!bDone) {
		bDone = true;
		for (int v1 = 1; v1 < graph.size(); v1++) {
			if (!bIsTerminal[v1] && graph[v1].size() == 1) {
				auto aux = graph[v1][0];
				graph[v1].clear();
				auto toDel = find(graph[aux.first].begin(), graph[aux.first].end(), std::make_pair(v1, aux.second));
				if (!graph[aux.first].empty()) {
					graph[aux.first].erase(toDel);
				}
				bDone = false;
			}
		}
	}
	updateLengthAndEdgeCount();
}

void FGraph::updateLengthAndEdgeCount()
{
	length = 0;
	edgeCount = 0;
	for (int i = 0; i < graph.size(); i++) {
		if (!graph[i].empty()) {
			edgeCount++;
			for (auto & e : graph[i]) {
				length += e.second;
			}
		}
	}
	length /= 2;
}


void FGraph::printGraph()
{
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			std::cout << i << " " << graph[i][j].first << " " << graph[i][j].second << "\n";
		}
	}
	std::cout << "V: " << vertexCount << "\tE: " << edgeCount << "\t T: " << terminalCount << "\t Length: " << length << "\n";
}
