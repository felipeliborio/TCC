#pragma once
#include <vector>

void BuildMaxHeap(std::vector<int> & Items);
void InsertMaxHeap(std::vector<int> & Heap, int Item);
void ConcatenateMaxHeap(std::vector<int> & HeapA, std::vector<int> & HeapB);

void BuildMinHeap(std::vector<int> & Items);
int ExtractMinHeap(std::vector<int> & Heap, int Root);

void HeapSort(std::vector<int> & Heap);






template <class T>
struct FHeap {
	int end = -1;
	std::vector<T> heap;
};

template<class T>
void heapify(FHeap<T> & heap, int root);
template<class T>
void heapBuild(FHeap<T>& heap);
template<class T>
void heapInsert(FHeap<T>& heap, T & item);
template<class T>
void heapConcatenate(FHeap<T>& heapA, FHeap<T>& heapB);
template<class T>
T heapExtract(FHeap<T>& heap, int root);
template<class T>
void heapSort(FHeap<T> & heap);


template<class T>
void heapify(FHeap<T> & heap, int root)
{
	int left = (root << 1) + 1;
	int right = (root << 1) + 2;

	int smallest = root;

	if (left <= heap.end && heap.heap[left][1] < heap.heap[smallest]) {
		smallest = left;
	}
	if (right <= heap.end && heap.heap[right][1] < heap.heap[smallest]) {
		smallest = right;
	}
	if (smallest != root) {
		std::swap(heap.heap[root], heap.heap[smallest]);
		heapify<T>(heap, smallest);
	}
}

template<class T>
void heapBuild(FHeap<T>& heap)
{
	for (int i = (heap.end - 1) >> 1; i > 0; i--) {
		heapify<T>(heap, i);
	}
}

template<class T>
void heapInsert(FHeap<T>& heap, T & item)
{
	heap.heap.push_back(item);
	heap.end++;

	int root = (heap.end - 1) >> 1;
	int child = heap.end;

	while (root >= heap.begin) {
		if (heap.heap[root] <= heap.heap[child]) {
			break;
		}
		else {
			std::swap(heap.heap[root], heap.heap[child]);
			child = root;
			root = root >> 1;
		}
	}
}

template<class T>
void heapConcatenate(FHeap<T>& heapA, FHeap<T>& heapB)
{
	heapA.heap.insert(heapA.heap.end(), heapB.heap.begin(), heapB.heap.begin() + heapB.end);
	heapA.end += heapB.end;
	heapBuild<T>(heapA);
}

template<class T>
T heapExtract(FHeap<T>& heap, int root)
{
	T oldRoot = heap.heap[root];
	std::swap(heap.heap[root], heap.heap[heap.end]);
	heap.heap.pop_back();
	heap.end--;
	heapify<T>(heap, root);
	return oldRoot;
}

template<class T>
void heapSort(FHeap<T> & heap)
{
	heapBuild(heap);
	while (heap.end > 0) {
		std::swap(heap.heap[0], heap.heap[heap.end]);
		heap.end--;
		heapify<T>(heap, 0);
	}
	heap.end = (int)heap.heap.size();
}
