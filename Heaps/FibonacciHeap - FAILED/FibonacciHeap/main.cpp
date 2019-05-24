#include "FFibonacciHeap.h"
#include "heap_sort.h"
#include <algorithm>

int main()
{
	

	std::deque<int> heapAElements = { 2, 7, 29, 10, 14, 11, 12, 43, 4, 7 };
	std::deque<int> heapBElements = { 3, 1, 5, 6, 20, 31, 11, 89 };
	
	FFibonacciHeap HeapA(heapAElements), HeapB(heapBElements);

	std::deque<int> toSort;
	for (int i = 0; i < 500; i++) {
		toSort.push_back(rand() % 1000000);
	}

	std::cout << "\n\nHeap sort:\n\n";
	heap_sort(toSort);
	std::sort(toSort.begin(), toSort.end());
	for (auto element : toSort) {
		std::cout << element << " ";
	}
	std::cout << "\n";
	
	return 0;
}
