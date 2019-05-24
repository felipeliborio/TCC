#include "AlphaKruskalTCP.h"
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>

int Find(int Vertex, std::deque<int> & Id)
{
	if (Id[Vertex] == -1 || Id[Vertex] == Vertex) return Vertex;
	return Id[Vertex] = Find(Id[Vertex], Id);
}

void Union(int Vertex1, int Vertex2, std::deque<int> & Id, std::deque<int> & Size)
{
	Vertex1 = Find(Vertex1, Id);
	Vertex2 = Find(Vertex2, Id);
	if (Vertex1 != Vertex2) {
		if (Size[Vertex1] < Size[Vertex2]) std::swap(Vertex1, Vertex2);
		Id[Vertex2] = Vertex1;
		Size[Vertex1] += Size[Vertex2];
	}
}

bool Shorter(std::deque<int> const & I, std::deque<int> const & J) 
{ 
	return I[2] < J[2]; 
}

TEdges Kruskal(TEdges Edges, std::unordered_set<int> const & Terminals, int const & VertexCount,
	int const & MaxL, int const & MaxR, float const & Alpha)
{
	TEdges Output;

	std::sort(Edges.begin(), Edges.end(), Shorter);
	std::deque<bool> MST;
	MST.resize(Edges.size(), false);
	std::deque<int> Id;
	Id.resize(VertexCount + 1, -1);
	std::deque<int> ClassSize;
	ClassSize.resize(VertexCount + 1, 1);

	int LCount = 0;
	int RCount = 0;
	std::unordered_map<int, std::unordered_map<int, int>> MSTEdgeMap;
	int EdgeCount = (int)Edges.size();
	int ChosenEdge;
	for (int Edge = 0; Edge < EdgeCount; Edge++) {

		int AlphaEdge = int((EdgeCount - Edge) * Alpha);
		if (AlphaEdge > 0) {
			ChosenEdge = Edge + (rand() % AlphaEdge);
		}
		else {
			ChosenEdge = Edge;
		}

		if (Find(Edges[ChosenEdge][0], Id) == Find(Edges[ChosenEdge][1], Id)) {
			std::swap(Edges[Edge], Edges[ChosenEdge]);
			std::swap(MST[Edge], MST[ChosenEdge]);
			continue;
		}

		// Ignores edge if it breaks L or R restrictions ****************************
		if (Terminals.find(Edges[ChosenEdge][0]) == Terminals.end()) {
			int ChosenVertexDegree = MSTEdgeMap[Edges[ChosenEdge][0]].size();
			if ((LCount >= MaxL && ChosenVertexDegree == 1) || (RCount >= MaxR && ChosenVertexDegree >= 1)) {
				std::swap(Edges[Edge], Edges[ChosenEdge]);
				std::swap(MST[Edge], MST[ChosenEdge]);
				continue;
			}
		}
		if (Terminals.find(Edges[ChosenEdge][1]) == Terminals.end()) {
			int ChosenVertexDegree = MSTEdgeMap[Edges[ChosenEdge][1]].size();
			if ((LCount >= MaxL && ChosenVertexDegree == 1) || (RCount >= MaxR && ChosenVertexDegree >= 1)) {
				std::swap(Edges[Edge], Edges[ChosenEdge]);
				std::swap(MST[Edge], MST[ChosenEdge]);
				continue;
			}
		}
		//*********************************************************************

		// Increase L and R count *********************************************
		int CVOldDegree = MSTEdgeMap[Edges[ChosenEdge][0]].size();
		MSTEdgeMap[Edges[ChosenEdge][0]][Edges[ChosenEdge][1]] = Edges[ChosenEdge][2];
		if (CVOldDegree == 1 && MSTEdgeMap[Edges[ChosenEdge][1]].size() == 2) {
			LCount++;
		}
		else if (CVOldDegree == 2 && MSTEdgeMap[Edges[ChosenEdge][1]].size() == 3) {
			LCount--;
			RCount++;
		}
		CVOldDegree = MSTEdgeMap[Edges[ChosenEdge][1]].size();
		MSTEdgeMap[Edges[ChosenEdge][1]][Edges[ChosenEdge][0]] = Edges[ChosenEdge][2] * (-1);
		if (CVOldDegree == 1 && MSTEdgeMap[Edges[ChosenEdge][1]].size() == 2) {
			LCount++;
		}
		else if (CVOldDegree == 2 && MSTEdgeMap[Edges[ChosenEdge][1]].size() == 3) {
			LCount--;
			RCount++;
		}
		//*********************************************************************

		Union(Edges[ChosenEdge][0], Edges[ChosenEdge][1], Id, ClassSize);
		MST[ChosenEdge] = true;
		std::swap(Edges[Edge], Edges[ChosenEdge]);
		std::swap(MST[Edge], MST[ChosenEdge]);
	}
	std::unordered_set<int> TerminalClasses;
	for (auto const & Terminal : Terminals) {
		TerminalClasses.insert(Find(Terminal, Id));
	}
	if (TerminalClasses.size() > 1) {
		return Output;
	}
	for (int I = 0; I < MST.size(); I++) {
		if (MST[I]) Output.push_back(Edges[I]);
	}
	return Output;
}
