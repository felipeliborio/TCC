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

* This project contains an implementation of a binary heap and a heapsort
* algorithm, along with some testing procedures.
*/

#define OUT//just to remember when something is passed as reference
#define SIZE

#include "binary_heap.h"
#include <iostream>
#include <algorithm>

int main()
{
	int vec[] = { SIZE 11, 16, 32, 9, 7, 3, 13, 11, 37, 36, 47, 51 };
	//First element stores heap size. So that less sum/subtract operations are
	//made when accounting for a 0 first index instead of an 1.

	std::vector<int> TestMaxHeap(vec, vec + sizeof(vec)/sizeof(int));//could had done it directly passing { values }
	BuildMaxHeap(OUT TestMaxHeap);
	for (auto Number : TestMaxHeap) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	InsertMaxHeap(OUT TestMaxHeap, 53);
	for (auto Number : TestMaxHeap) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	ConcatenateMaxHeap(OUT TestMaxHeap, std::vector<int>({ SIZE 7, 57, 67, 71, 83, 91, 97, 107 }));
	for (auto Number : TestMaxHeap) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	std::vector<int> TestMinHeap(vec, vec + sizeof(vec) / sizeof(int));
	BuildMinHeap(OUT TestMinHeap);
	for (auto Number : TestMinHeap) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	ExtractMinHeap(OUT TestMinHeap, 1);
	for (auto Number : TestMinHeap) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	std::cout << ExtractMinHeap(OUT TestMinHeap, 3) << "\n";
	for (auto Number : TestMinHeap) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	std::vector<int> toSort;
	toSort.push_back(1000000);
	for (int i = 0; i < 1000000; i++) {
		toSort.push_back(rand() * (rand() % 10000));
	}
	
	auto toSort2 = toSort;
	sort(toSort2.begin(), toSort2.end());
	HeapSort(OUT toSort);
	for (auto Number : toSort) {
		std::cout << Number << " ";
	}
	std::cout << "\n";

	return 0;
}
