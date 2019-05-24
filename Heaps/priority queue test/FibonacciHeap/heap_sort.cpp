#include "heap_sort.h"
#include <functional>


void heap_sort(std::vector<int> & elements)
{
	std::priority_queue<int, std::vector<int>, std::greater<int>> queue(elements.begin(), elements.end());
	

	int i = 0, s = elements.size();
	while (i < s) {
		elements[i] = queue.top();
		queue.pop();
		i++;
	}
}
