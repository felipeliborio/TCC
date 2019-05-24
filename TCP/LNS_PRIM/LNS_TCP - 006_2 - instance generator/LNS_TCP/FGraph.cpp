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
	else if (instanceData[0].size() == 4) {
		vertexCount = instanceData[0][0];
		edgeCount = instanceData[0][1];
		maxL = instanceData[0][2];
		maxR = instanceData[0][3];
		graph.resize(vertexCount + 1);
		bIsTerminal.resize(vertexCount + 1, false);
	}

	int i = 1;
	for (; i < instanceData.size(); i++) {
		if (instanceData[i].size() == 3) {
			addEdge(instanceData[i]);
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

void FGraph::addEdge(std::vector<int> & edge) 
{
	if (std::find(graph[edge[0]].begin(), graph[edge[0]].end(), 
		std::make_pair( edge[1], edge[2] )) == graph[edge[0]].end()) {

		graph[edge[0]].push_back(std::make_pair(edge[1], edge[2]));
		graph[edge[1]].push_back(std::make_pair(edge[0], edge[2]));
		length += edge[2];
	}
}

void FGraph::deleteEdge(std::vector<int> & edge)
{
	for (int i = 0; i < graph[edge[0]].size(); i++) {
		if (graph[edge[0]][i].first == edge[1]) {
			graph[edge[0]].erase(graph[edge[0]].begin() + i);
			break;
		}
	}

	for (int i = 0; i < graph[edge[1]].size(); i++) {
		if (graph[edge[1]][i].first == edge[0]) {
			graph[edge[1]].erase(graph[edge[1]].begin() + i);
			break;
		}
	}
}

void FGraph::simplifyInstance()
{
	bool bDone = false;
	while (!bDone) {
		bDone = true;
		for (int v1 = 0; v1 < graph.size(); v1++) {
			if (!bIsTerminal[v1] && graph[v1].size() == 1) {
				auto aux = graph[v1][0];
				graph[v1].clear();
				auto toDel = find(graph[aux.first].begin(), graph[aux.first].end(), std::make_pair(v1, aux.second));
				if (toDel != graph[aux.first].end()) {
					graph[aux.first].erase(toDel);
				}
				bDone = false;
			}
		}
	}
	updateLength();
}

void FGraph::updateLength()
{
	length = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (auto & e : graph[i]) {
			length += e.second;
		}
	}
	length /= 2;
}

void FGraph::printGraph()
{
	std::cout << "v: " << vertexCount << "\te: " << edgeCount << "\tt: " << terminalCount << "\t len: " << length << "\n";
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			std::cout << i << " " << graph[i][j].first << " " << graph[i][j].second << "\n";
		}
	}
}
