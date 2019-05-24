#include "binomial_heap.h"
#include <iostream>

int main()
{
	FBinomialHeap heap1;
	FBinomialHeap heap2;
	FBinomialHeap heap3;
	heap1.createSampleHeap2();
	heap2.createSampleHeap3();
	heap1.merge(heap2);
	heap1.printHeap();
	heap1.deleteMin();
	heap1.printHeap();
	return 0;
}
