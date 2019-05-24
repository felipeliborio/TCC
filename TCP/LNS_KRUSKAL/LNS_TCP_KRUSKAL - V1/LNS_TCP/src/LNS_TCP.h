#pragma once
#include "AlphaKruskalTCP.h"

using TEdges = std::deque<std::deque<int>>;
void CutInstance(TEdges & Instance, std::unordered_set<int> const & Terminals);
TEdges LNS_TCP(FInstance & Instance, TEdges & Solution);
int GraphLength(TEdges & Solution);
