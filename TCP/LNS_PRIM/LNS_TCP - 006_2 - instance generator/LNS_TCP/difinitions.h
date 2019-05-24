#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <filesystem>
#include <random>
#include <math.h>
#include <algorithm>

typedef std::pair<int, int> iPair;//integer pair for the graph representation

struct FVertex {
	int vertex = -1;
	int distanceTo = INT32_MAX;
};

struct FHeap {
	int end = -1;
	std::vector<FVertex> elements;
};

