#pragma once
#include <deque>
#include <unordered_set>
#include <concurrent_unordered_set.h>
#include <set>


using TEdges = std::deque<std::deque<int>>;

struct FInstance {
	int VertexCount = 0;
	int EdgeCount = 0;
	int TerminalCount = 0;
	int MaxL = INT32_MAX;
	int MaxR = INT32_MAX;
	TEdges Edges;
	std::unordered_set<int> Terminals;
};


TEdges Kruskal(TEdges Edges, std::unordered_set<int> const & Terminals, int const & VertexCount,
	int const & MaxL, int const & MaxR, float const & Alpha);
