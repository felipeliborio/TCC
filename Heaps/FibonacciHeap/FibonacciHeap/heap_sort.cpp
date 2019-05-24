#include "heap_sort.h"

void heap_sort(std::deque<int> & elements)
{
	fib_heap::FibHeap heap;

	for (auto & key : elements) {
		heap.insert(key);
	}

	int i = 0, s = elements.size();
	while (i < s) {
		elements[i++] = heap.extract_min();
	}
}
