#include "AlphaKruskal.h"
#include <algorithm>
#include <stdlib.h>

int Find(int P, std::deque<int> & Id)
{
	if (Id[P] == -1 || Id[P] == P) return P;
	return Id[P] = Find(Id[P], Id);
}

void Union(int P, int Q, std::deque<int> & Id, std::deque<int> & Size)
{
	P = Find(P, Id);
	Q = Find(Q, Id);
	if (P != Q) {
		if (Size[P] < Size[Q]) std::swap(P, Q);
		Id[Q] = P;
		Size[P] += Size[Q];
	}
}

bool Shorter(std::deque<int> const & I, std::deque<int> const & J) 
{ 
	return I[2] < J[2]; 
}

TEdges Kruskal(TEdges Edges, int const & VertexCount, float const & Alpha)
{
	TEdges Output;
	std::sort(Edges.begin(), Edges.end(), Shorter);
	std::deque<bool> MST;
	MST.resize(Edges.size(), false);
	std::deque<int> Id;
	Id.resize(VertexCount + 1, -1);
	std::deque<int> Size;
	Size.resize(VertexCount + 1, 1);
	int DSize = (int)Edges.size();
	int R;
	for (int E = 0; E < DSize; E++) {
		int AlphaEdge = int((DSize - E) * Alpha);
		if (AlphaEdge > 0) {
			R = E + (rand() % AlphaEdge);
		}
		else {
			R = E;
		}
		if (Find(Edges[R][0], Id) == Find(Edges[R][1], Id)) {
			std::swap(Edges[E], Edges[R]);
			std::swap(MST[E], MST[R]);
			continue;
		}
		Union(Edges[R][0], Edges[R][1], Id, Size);
		MST[R] = true;
		std::swap(Edges[E], Edges[R]);
		std::swap(MST[E], MST[R]);
	}
	for (int I = 0; I < MST.size(); I++) {
		if (MST[I]) Output.push_back(Edges[I]);
	}
	//std::sort(Output.begin(), Output.end());
	return Output;
}
