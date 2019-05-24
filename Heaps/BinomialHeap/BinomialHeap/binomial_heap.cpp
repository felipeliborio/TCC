#include "binomial_heap.h"

void FBinomialHeap::initializeNode(FNodePtr node, int data, int degree)
{
	node->data = data;
	node->degree = degree;
	node->parent = nullptr;
	node->child = nullptr;
	node->sibling = nullptr;
}

void FBinomialHeap::linkBinomialTrees(FNodePtr treeA, FNodePtr treeB) {
	treeB->parent = treeA;
	treeB->sibling = treeA->child;
	treeA->child = treeB;
	treeA->degree++;
}

FBinomialHeap::FBinomialHeap()
{
	head = nullptr;
}

FNodePtr FBinomialHeap::findMinimum()
{
	int min = INT32_MAX;
	FNodePtr currPtr = head;
	FNodePtr minPtr = nullptr;

	while (currPtr != nullptr) {
		if (currPtr->data < min) {
			min = currPtr->data;
			minPtr = currPtr;
		}
		currPtr = currPtr->sibling;
	}
	return minPtr;
}

void FBinomialHeap::insert(int data)
{
	FBinomialHeap newHeap;
	FNodePtr node = new FNode;
	initializeNode(node, data, 0);
	newHeap.setHead(node);
	merge(newHeap);
}

void FBinomialHeap::printHeap()
{
	FNodePtr currPtr = head;
	while (currPtr != nullptr) {
		std::cout << "B" << currPtr->degree << std::endl;
		std::queue<FNodePtr> queue;
		queue.push(currPtr);
		while (!queue.empty()) {
			FNodePtr pointer = queue.front();
			queue.pop();
			std::cout << pointer->data << " ";

			if (pointer->child != nullptr) {
				FNodePtr tempPtr = pointer->child;
				while (tempPtr != nullptr) {
					queue.push(tempPtr);
					tempPtr = tempPtr->sibling;
				}
			}
		}
		currPtr = currPtr->sibling;
		std::cout << "\n\n";
	}
}

FNodePtr FBinomialHeap::getHead() {
	return head;
}

void FBinomialHeap::setHead(FNodePtr head)
{
	this->head = head;
}

void FBinomialHeap::merge(FBinomialHeap heapB)
{
	FNodePtr curr1 = getHead();
	FNodePtr curr2 = heapB.getHead();
	FNodePtr curr3 = nullptr;
	FNodePtr temp = nullptr;

	//defines the head of the heap
	if (curr1->degree <= curr2->degree) {
		curr3 = curr1;
		curr1 = curr1->sibling;
	}
	else {
		curr3 = curr2;
		curr2 = curr2->sibling;
	}

	temp = curr3;//stores the head

	//merge the heaps without merging equal sized trees
	while (curr1 != nullptr && curr2 != nullptr) {
		if (curr1->degree <= curr2->degree) {
			curr3->sibling = curr1;
			curr1 = curr1->sibling;
		}
		else {
			curr3->sibling = curr2;
			curr2 = curr2->sibling;
		}
		curr3 = curr3->sibling;
	}

	if (curr1 != nullptr) {
		while (curr1 != nullptr) {
			curr3->sibling = curr1;
			curr1 = curr1->sibling;
			curr3 = curr3->sibling;
		}
	}
	
	if (curr2 != nullptr) {
		while (curr2 != nullptr) {
			curr3->sibling = curr2;
			curr2 = curr2->sibling;
			curr3 = curr3->sibling;
		}
	}
	/*********************************************************/

	curr3 = temp;
	// merge the trees with same degree
	FNodePtr prev = nullptr;
	FNodePtr next = curr3->sibling;

	while (next != nullptr) {
		// if two adjacent tree roots have different degree or 3 consecutive roots have same degree
		// move to the next tree
		if ((curr3->degree != next->degree) ||
			(next->sibling != nullptr && curr3->degree == next->sibling->degree)) {
			prev = curr3;
			curr3 = next;
		}
		else {
			//othrewise repeatdly merge binomial trees with the same degree
			if (curr3->data <= next->data) {
				curr3->sibling = next->sibling;
				FBinomialHeap::linkBinomialTrees(curr3, next);
			}
			else {
				if (prev == nullptr) {
					temp = next;
				}
				else {
					prev->sibling = next;
				}
				FBinomialHeap::linkBinomialTrees(next, curr3);
				curr3 = next;
			}
		}
		next = curr3->sibling;
	}

	setHead(temp);
}

FNodePtr FBinomialHeap::deleteMin()
{
	FNodePtr curr = head;
	FNodePtr prevMin = nullptr;
	FNodePtr minPtr = nullptr;
	FNodePtr prevPtr = nullptr;
	int min = INT32_MAX;

	while (curr != nullptr) {
		if (curr->data <= min) {
			min = curr->data;
			prevMin = prevPtr;
			minPtr = curr;
		}
		prevPtr = curr;
		curr = curr->sibling;
	}

	//delete the node pointed by minPtr
	if (prevMin != nullptr && minPtr->sibling != nullptr) {
		prevMin->sibling = minPtr->sibling;
	}
	else if (prevMin != nullptr && minPtr->sibling == nullptr) {
		prevMin->sibling = nullptr;
	}

	//remove the reference from its children
	FNodePtr childPtr = minPtr->child;
	while (childPtr != nullptr) {
		childPtr->parent = nullptr;
		childPtr = childPtr->sibling;
	}

	// reverse the order
	std::stack<FNodePtr> s;
	childPtr = minPtr->child;
	while (childPtr != nullptr) {
		s.push(childPtr);
		childPtr = childPtr->sibling;
	}
	curr = s.top();
	FNodePtr temp = curr;
	s.pop();

	while (!s.empty()) {
		curr->sibling = s.top();
		s.pop();
		curr = curr->sibling;
	}

	curr->sibling = nullptr;

	FBinomialHeap h;
	h.setHead(temp);

	merge(h);

	return minPtr;
}

void FBinomialHeap::buildFromDeque(std::deque<int> elements)
{
}
