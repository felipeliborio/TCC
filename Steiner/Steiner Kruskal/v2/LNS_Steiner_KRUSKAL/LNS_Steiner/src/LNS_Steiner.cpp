#include "LNS_Steiner.h"
#include "AlphaKruskal.h"

std::deque<int> GetVertices(TEdges & Instance);
void Merge(TEdges & NewSolution, TEdges & Neighborhood);
int GraphLength(TEdges & Solution);
TEdges GetEdgesConnectedToVertex(TEdges const & Edges, int Vertex);
void InsertEdge(TEdges & Instance, std::deque<int> & Edge);
void RemoveVertex(TEdges & Instance, int Vertex);
void ExpandNeighborhood(TEdges & Neighborhood, TEdges const & InstanceEdges, TEdges const & LastSol, float Destruction);
int GetVertexCount(TEdges const & Edges);

TEdges LNS_Steiner(FInstance & Instance, TEdges & Solution)
{
	TEdges BestSolution = Solution;
	TEdges NewSolution = Solution;
	std::unordered_set<int> Terminals = Instance.Terminals;
	TEdges Neighborhood;
	
	for (int i = 0; i < Instance.VertexCount; i++) {
		ExpandNeighborhood(Neighborhood, Instance.Edges, NewSolution,((rand() % 14 + 6) / 20.0));
		Merge(NewSolution, Neighborhood);
		NewSolution = Kruskal(NewSolution, Instance.VertexCount, ((rand() % 8 + 2) / 20.0));
		CutInstance(NewSolution, Terminals);

		if (GraphLength(NewSolution) < GraphLength(BestSolution)) {
			BestSolution = NewSolution;
		}
		else {
			NewSolution = BestSolution;
		}
		Neighborhood = TEdges();
	}
	return BestSolution;
}

void ExpandNeighborhood(TEdges & Neighborhood, TEdges const & InstanceEdges, TEdges const & LastSol, float Destruction)
{
	int InstEdgeCount = (int)LastSol.size();
	int MaxSize = (int)((float)InstEdgeCount * Destruction);
	while (Neighborhood.size() < MaxSize) {
		int Chosen = LastSol[rand() % InstEdgeCount][rand() % 2];
		TEdges EdgesToInsert = GetEdgesConnectedToVertex(InstanceEdges, Chosen);
		for (auto Edge : EdgesToInsert) {
			InsertEdge(Neighborhood, Edge);
		}
	}
}

/*
void ExpandNeighborhood(TEdges & Neighborhood, TEdges const & InstanceEdges, float Destruction)
{
	int InstEdgeCount = (int)InstanceEdges.size();
	int MaxSize = (int) ((float)InstEdgeCount * Destruction);
	while (Neighborhood.size() < MaxSize) {
		int Chosen = InstanceEdges[rand() % InstEdgeCount][rand() % 2];
		TEdges EdgesToInsert = GetEdgesConnectedToVertex(InstanceEdges, Chosen);
		for (auto Edge : EdgesToInsert) {
			InsertEdge(Neighborhood, Edge);
		}
	}
}*/

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
	for (auto Edge : Addon) {
		InsertEdge(Master, Edge);
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

std::deque<int> GetVertices(TEdges & Edges)
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
	for (auto & Edge : Edges) {
		if (Edge[0] == Vertex || Edge[1] == Vertex) {
			Output.push_back(Edge);
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
	int Size = Vertices.size();
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
	int Size = Edges.size();
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

/* Old code for LNS
void ExpandNeighborhood(FInstance & Neighborhood, FInstance & Instance, std::deque<int> SolutionVertices, float const & Destruction) {
	while (Neighborhood.GetGraphPointer()->size() < SolutionVertices.size() * Destruction) {
		int Chosen = SolutionVertices[rand() % SolutionVertices.size()];
		std::deque<FEdge> EdgesToInsert = Instance.GetEdgesConnectedToVertex(Chosen);
		for (auto Edge : EdgesToInsert) {
			Neighborhood.InsertEdge(Edge);
		}
	}
}

FInstance LNS(FInstance & Instance, FInstance & Solution) {
	FInstance BestSolution = Solution;
	FInstance NewSolution = Solution;
	std::deque<int> SolutionVertices = Solution.GetVertices();
	FInstance Neighborhood = FInstance();
	for (int i = 0; i < Instance.GetTerminalsPointer()->size() + 3000; i++) {
		ExpandNeighborhood(Neighborhood, Instance, SolutionVertices, (float)(((rand() % 16) + 3) / 20.0));
		NewSolution.Merge(Neighborhood);
		NewSolution.SetMaxLinks((int)(0.7 * Instance.GetMaxLinks()) + (int)(((rand() % 17) * 0.1) * Instance.GetMaxLinks()));
		NewSolution.SetMaxRouters((int)(0.7 * Instance.GetMaxRouters()) + (int)(((rand() % 17) * 0.1) * Instance.GetMaxRouters()));
		//std::cout << NewSolution.GetMaxLinks() << std::endl;
		NewSolution = MPrimTCP(NewSolution, (float)(((rand() % 24)) / 30.0));
		if (NewSolution.GetLength() < BestSolution.GetLength()) {
			BestSolution = NewSolution;
		}
		else {
			NewSolution = BestSolution;
		}
		Neighborhood = FInstance();
	}
	return BestSolution;
}
*/
