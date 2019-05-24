#include "FibonacciHeap.h"
#include "heap_sort.h"
#include <algorithm>

int main()
{	
	std::deque<int> toSort;
	for (int i = 0; i < 1000000; i++) {
		toSort.push_back(rand());
	}
	auto toSort2 = toSort;


	std::cout << "\n\nHeap sort:\n\n";
	std::sort(toSort2.begin(), toSort2.end());
	heap_sort(toSort);

	for (auto element : toSort) {
		std::cout << element << " ";
	}
	std::cout << "\n";
	
	return 0;
}
