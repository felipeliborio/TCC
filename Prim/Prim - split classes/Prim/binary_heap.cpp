#include "binary_heap.h"

void heapify(FHeap & heap, int root)
{
	int left = (root * 2) + 1;
	int right = (root * 2) + 2;

	int smallest = root;

	if (left <= heap.end && heap.elements[left].distanceTo < heap.elements[smallest].distanceTo) {
		smallest = left;
	}
	if (right <= heap.end && heap.elements[right].distanceTo < heap.elements[smallest].distanceTo) {
		smallest = right;
	}
	if (smallest != root) {
		std::swap(heap.elements[root], heap.elements[smallest]);
		heapify(heap, smallest);
	}
}

void heapBuild(FHeap& heap)
{
	for (int i = (heap.end - 1) / 2; i > 0; i--) {
		heapify(heap, i);
	}
	heap.end = heap.elements.size() - 1;
}

void heapInsert(FHeap& heap, FVertex & item)
{
	heap.elements.push_back(item);
	heap.end++;

	int root = (heap.end - 1) / 2;
	int child = heap.end;

	while (root >= 0) {
		if (heap.elements[root].distanceTo <= heap.elements[child].distanceTo) {
			break;
		}
		else {
			std::swap(heap.elements[root], heap.elements[child]);
			child = root;
			root = (root - 1) / 2;
		}
	}
}

void heapConcatenate(FHeap& heapA, FHeap& heapB)
{
	heapA.elements.insert(heapA.elements.end(), heapB.elements.begin(), heapB.elements.begin() + heapB.end);
	heapBuild(heapA);
}

FVertex heapExtract(FHeap& heap, int root)
{
	if (heap.end < 0) {
		return FVertex();
	}
	FVertex oldRoot = heap.elements[root];
	std::swap(heap.elements[root], heap.elements[heap.end]);
	heap.elements.pop_back();
	heap.end--;
	heapify(heap, root);
	return oldRoot;
}

void heapSort(FHeap & heap)
{
	heapBuild(heap);
	while (heap.end > 0) {
		std::swap(heap.elements[0], heap.elements[heap.end]);
		heap.end--;
		heapify(heap, 0);
	}
	heap.end = (int)heap.elements.size();
}
