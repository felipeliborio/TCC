#pragma once
#include <deque>
#include <unordered_set>


using TEdges = std::deque<std::deque<int>>;

struct FInstance {
	int VertexCount = 0;
	int EdgeCount = 0;
	int TerminalCount = 0;
	TEdges Edges;
	std::unordered_set<int> Terminals;
};

TEdges Kruskal(TEdges Edges, int const & VertexCount, float const & Alpha);
