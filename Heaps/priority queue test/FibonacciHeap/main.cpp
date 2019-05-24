#include "heap_sort.h"
#include <algorithm>
#include<iterator>

int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

int main()
{	
	std::vector<int> toSort;//deque is almost trice as slow as vector for sort and priority_queue and doesn't work with qsort
	for (int i = 0; i < 1000000; i++) {
		toSort.push_back(rand() * (rand() % 10000));
	}
	auto toSort2 = toSort;
	auto toSort3 = toSort;

	std::cout << "\n\nHeap sort:\n\n";
	std::qsort(&toSort2[0], toSort2.size(), sizeof(int), compare);
	std::sort(toSort3.begin(), toSort3.end());
	heap_sort(toSort);

	for (auto element : toSort) {
		std::cout << element << " ";
	}
	std::cout << "\n";
	
	return 0;
}
