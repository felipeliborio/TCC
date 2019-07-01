#include "LNS_TCP.h"
#include "AlphaKruskalTCP.h"

const int Disconnected = INT32_MIN;

std::deque<int> GetVertices(TEdges const & Instance);
void Merge(TEdges & NewSolution, TEdges & Neighborhood);
TEdges GetEdgesConnectedToVertex(TEdges const & Edges, int Vertex);
void InsertEdge(TEdges & Instance, std::deque<int> & Edge);
void RemoveVertex(TEdges & Instance, int Vertex);
void ExpandNeighborhood(TEdges & Neighborhood, TEdges const & InstanceEdges, 
	int const & InstanceEdgeCount, TEdges const & LastSol, float Destruction);
int GetVertexCount(TEdges const & Edges);
TEdges GetEdgeMatrix(TEdges const & Edges, int const & VertexCount);

TEdges LNS_TCP(FInstance & Instance, TEdges & Solution)
{
	TEdges BestSolution = Solution;
	int BestSolutionLength = GraphLength(BestSolution);
	TEdges NewSolution = Solution;
	int NewSolutionLength;
	std::unordered_set<int> Terminals = Instance.Terminals;
	TEdges Neighborhood;
	TEdges InstEdgeMatrix = GetEdgeMatrix(Instance.Edges, Instance.VertexCount);
	int MaxL = Instance.MaxL;
	int MaxR = Instance.MaxR;
	int NoEnhancementIt = 0;
	for (int i = 0; i < Instance.TerminalCount + 2000; i++) {
	//while (NoEnhancementIt < Instance.VertexCount + Instance.TerminalCount) {
		ExpandNeighborhood(Neighborhood, InstEdgeMatrix, Instance.EdgeCount, NewSolution, (float) ((rand() % 2 + 2) / 20.0));
		Merge(NewSolution, Neighborhood);
		NewSolution = Kruskal(NewSolution, Instance.Terminals, Instance.VertexCount, 
			MaxL, MaxR, (float)((rand() % 3 + 1) / 10.0));
		//MaxL = (int)((rand() % 10) * 0.2 + 0.4) * Instance.MaxL;
		//MaxR = (int)((rand() % 10) * 0.2 + 0.4) * Instance.MaxR;

		if (NewSolution.size() == 0) {
			NewSolution = BestSolution;
			NoEnhancementIt++;
			continue;
		}
		CutInstance(NewSolution, Terminals);
		NewSolutionLength = GraphLength(NewSolution);
		if (NewSolutionLength < BestSolutionLength) {
			BestSolution = NewSolution;
			BestSolutionLength = NewSolutionLength;
			NoEnhancementIt = 0;
		}
		else {
			NewSolution = BestSolution;
			NoEnhancementIt++;
		}
		Neighborhood = TEdges();
	}
	return BestSolution;
}

void ExpandNeighborhood(TEdges & Neighborhood, TEdges const & InstanceEdges, int const & InstanceEdgeCount,
	TEdges const & LastSol, float Destruction)
{
	std::deque<int> VertexList = GetVertices(LastSol);
	int MaxSize = (int)(InstanceEdges.size() * Destruction);
	std::unordered_set<int> Visited;
	while (Neighborhood.size() < MaxSize && Neighborhood.size() < 4 * LastSol.size()) {
		int CIndex = rand() % VertexList.size();
		int Chosen = VertexList[CIndex];
		TEdges EdgesToInsert = GetEdgesConnectedToVertex(InstanceEdges, Chosen);
		VertexList.erase(VertexList.begin() + CIndex);
		for (auto Edge : EdgesToInsert) {
			InsertEdge(Neighborhood, Edge);
			if (Visited.find(Edge[0]) == Visited.end()) {
				VertexList.push_back(Edge[0]);
				Visited.insert(Edge[0]);
			}
			if (Visited.find(Edge[1]) == Visited.end()) {
				VertexList.push_back(Edge[1]);
				Visited.insert(Edge[1]);
			}
		}
	}
}

TEdges GetEdgeMatrix(TEdges const & Edges, int const & VertexCount)
{
	TEdges Output;
	Output.resize(VertexCount + 1);//to access it mor intuitively
	for (auto & Vertex : Output) {
		Vertex.resize(VertexCount + 1, Disconnected);
	}
	for (auto & Edge : Edges) {
		Output[Edge[0]][Edge[1]] = Edge[2];
		Output[Edge[1]][Edge[0]] = (-1) * Edge[2];
	}
	return Output;
}

std::unordered_set<int> GetVerticesUS(TEdges const & Edges)
{
	std::unordered_set<int> Output;
	for (auto & Edge : Edges) {
		Output.insert(Edge[0]);
		Output.insert(Edge[1]);
	}
	return Output;
}

int GetVertexCount(TEdges const & Edges)
{
	std::unordered_set<int> Temp;
	for (auto & Edge : Edges) {
		Temp.insert(Edge[0]);
		Temp.insert(Edge[1]);
	}
	return (int)Temp.size();
}

void InsertEdge(TEdges & EdgeList, std::deque<int> & NewEdge)
{
	for (auto const & Edge : EdgeList) {
		if (Edge[0] == NewEdge[0] && Edge[1] == NewEdge[1]) {
			return;
		}
		else if (Edge[1] == NewEdge[0] && Edge[0] == NewEdge[1]) {
			return;
		}
	}
	EdgeList.push_back(NewEdge);
}

void Merge(TEdges & Master, TEdges & Addon)
{
	std::set<std::deque<int>> Temp;
	for (auto & Edge : Master) {
		Temp.insert(Edge);
	}
	for (auto & Edge : Addon) {
		Temp.insert(Edge);
	}
	Master.clear();
	for (auto Edge : Temp) {
		Master.push_back(Edge);
	}
}

int GraphLength(TEdges & Edges)
{
	int Output = 0;
	for (auto & Edge : Edges) {
		Output += Edge[2];
	}
	return Output;
}

std::deque<int> GetVertices(TEdges const & Edges)
{
	std::deque<int> Output;
	std::unordered_set<int> Temp;
	for (auto & Edge : Edges) {
		Temp.insert(Edge[0]);
		Temp.insert(Edge[1]);
	}
	for (auto Vertex : Temp) {
		Output.push_back(Vertex);
	}
	return Output;
}

TEdges GetEdgesConnectedToVertex(TEdges const & Edges, int Vertex)
{
	TEdges Output;
	for (int i = 0; i < Edges.size(); i++) {
		if (Edges[Vertex][i] != Disconnected) {
			if (Edges[Vertex][i] > 0) {
				Output.push_back({ Vertex, i, Edges[Vertex][i] });
			}
			else {
				Output.push_back({ i, Vertex, Edges[Vertex][i] * (-1) });
			}
		}
	}
	return Output;
}

int GetVertexDegree(TEdges & Edges, int Vertex)
{
	int Output = 0;
	for (auto & Edge : Edges) {
		if (Edge[0] == Vertex || Edge[1] == Vertex) {
			Output++;
		}
	}
	return Output;
}

/*
Remove de unnecessary vertices
TODO this is crap SLOW
*/
void CutInstance(TEdges & Edges, std::unordered_set<int> const & Terminals)
{
	std::deque<int> Vertices = GetVertices(Edges);
	int Size = (int)Vertices.size();
	int i;
	auto TEnd = Terminals.end();
	bool bDone = false;
	while (!bDone) {
		bDone = true;
		i = 0;
		while (i < Size) {
			if (Terminals.find(Vertices[i]) == TEnd) {
				if (GetVertexDegree(Edges, Vertices[i]) < 2) {
					RemoveVertex(Edges, Vertices[i]);
					bDone = false;
					Vertices.erase(Vertices.begin() + i);
					Size--;
					i--;
				}
			}
			i++;
		}
	}
}

void RemoveVertex(TEdges & Edges, int Vertex)
{
	int Size = (int)Edges.size();
	int i = 0;
	while (i < Size) {
		if (Edges[i][0] == Vertex || Edges[i][1] == Vertex) {
			Edges.erase(Edges.begin() + i);
			Size--;
			i--;
		}
		i++;
		continue;
	}
}

