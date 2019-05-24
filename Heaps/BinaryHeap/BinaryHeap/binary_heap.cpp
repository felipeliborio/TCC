/*
MIT License

Copyright (c) <2019> <Felipe Libório>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "binary_heap.h"

void MaxHeapify(std::vector<int> & Heap, int Root)
{
	int Left = 2 * Root;
	int Right = 2 * Root + 1;

	int Largest = Root;

	if (Left <= Heap[0] && Heap[Left] > Heap[Largest]) {
		Largest = Left;
	}
	if (Right <= Heap[0] && Heap[Right] > Heap[Largest]) {
		Largest = Right;
	}
	if (Largest != Root) {
		std::swap(Heap[Root], Heap[Largest]);
		MaxHeapify(Heap, Largest);
	}
}

void BuildMaxHeap(std::vector<int> & Items)
{
	for (int i = Items[0] / 2; i > 0; i--) {
		MaxHeapify(Items, i);
	}
}

void InsertMaxHeap(std::vector<int> & Heap, int Item)
{
	Heap.push_back(Item);
	Heap[0]++;

	int Root = Heap[0] / 2;
	int Child = Heap[0];
	while (Root >= 1)
	{
		if (Heap[Root] >= Heap[Child]) {
			break;
		}
		else {
			std::swap(Heap[Root], Heap[Child]);
			Child = Root;
			Root = Root >> 1;
		}
	}
}

void ConcatenateMaxHeap(std::vector<int> & HeapA, std::vector<int> & HeapB)
{

	HeapA.insert(HeapA.end(), ++HeapB.begin(), HeapB.end());
	HeapA[0] = HeapA.size() - 1;
	BuildMaxHeap(HeapA);
}



void MinHeapify(std::vector<int> & Heap, int Root)
{
	int Left = 2 * Root;
	int Right = 2 * Root + 1;

	int Smallest = Root;

	if (Left <= Heap[0] && Heap[Left] < Heap[Smallest]) {
		Smallest = Left;
	}
	if (Right <= Heap[0] && Heap[Right] < Heap[Smallest]) {
		Smallest = Right;
	}
	if (Smallest != Root) {
		std::swap(Heap[Root], Heap[Smallest]);
		MinHeapify(Heap, Smallest);
	}
}

void BuildMinHeap(std::vector<int> & Items)
{
	for (int i = Items[0] / 2; i > 0; i--) {
		MinHeapify(Items, i);
	}
}

int ExtractMinHeap(std::vector<int> & Heap, int Root)
{
	int RootValue = Heap[Root];
	std::swap(Heap[Root], Heap[Heap[0]]);
	Heap.pop_back();
	Heap[0]--;
	MinHeapify(Heap, Root);
	return RootValue;
}


void HeapSort(std::vector<int> & Heap)
{
	BuildMaxHeap(Heap);
	while (Heap[0] > 1) {
		std::swap(Heap[1], Heap[Heap[0]]);
		Heap[0]--;
		MaxHeapify(Heap, 1);
	}
	Heap[0] = Heap.size() - 1;
}

