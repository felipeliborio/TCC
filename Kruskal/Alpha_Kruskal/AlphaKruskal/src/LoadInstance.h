/*
Loads a Steiner problem instance from a file into an apropriate data structure.
*/
#pragma once

#include <deque>
using TInstData = std::deque<std::deque<int>>;
TInstData LoadInstance(const std::string FileDirectory);
