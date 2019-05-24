#pragma once
#include "AlphaKruskal.h"

using TEdges = std::deque<std::deque<int>>;
void CutInstance(TEdges & Instance, std::unordered_set<int> const & Terminals);
TEdges LNS_Steiner(FInstance & Instance, TEdges & Solution);
int GraphLength(TEdges & Solution);
