/*
Loads a Steiner problem instance from a file into an apropriate data structure.
*/
#pragma once

#include <deque>
using TEdges = std::deque<std::deque<int>>;
TEdges LoadInstance(const std::string FileDirectory);
