#include "heap_sort.h"

void heap_sort(std::deque<int> & elements)
{
	FFibonacciHeap heap(elements);
	for (int i = 0; i < elements.size(); i++) {
		elements[i] = heap.getMin()->key;
		heap.extractMin();
	}
}
