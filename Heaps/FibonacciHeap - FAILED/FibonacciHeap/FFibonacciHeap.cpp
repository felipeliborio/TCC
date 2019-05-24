#include "FFibonacciHeap.h"

FFibonacciHeap::FFibonacciHeap(std::deque<int> & const elements)
{
	initializeNode(head, INT32_MAX, head, head);
	min = head;
	if (!elements.empty()) {
		insertFromDeque(elements);
	}
}

FFibonacciHeap::~FFibonacciHeap()
{
}

void FFibonacciHeap::initializeNode(FNodePtr node, int key, FNodePtr lSib, FNodePtr rSib,
	FNodePtr child, FNodePtr parent, int degree, bool loser)
{
	node->key = key;
	node->degree = degree;
	node->loser = loser;
	node->lSibling = lSib;
	node->rSibling = rSib;
	node->child = child;
	node->parent = parent;
}

FNodePtr FFibonacciHeap::linkTrees(FNodePtr treeA, FNodePtr treeB)
{
	if (treeA->key > treeB->key) {
		std::swap(treeA, treeB);
	}

	treeB->lSibling->rSibling = treeB->rSibling;
	treeB->rSibling->lSibling = treeB->lSibling;

	if (treeA->rSibling == treeB) {
		treeA->rSibling = treeB->rSibling;
	}
	if (treeA->lSibling == treeB) {
		treeA->lSibling = treeB->lSibling;
	}

	treeB->parent = treeA;
	if (treeA->child == nullptr) {
		treeB->lSibling = treeB;
		treeB->rSibling = treeB;
	}
	else {
		treeB->lSibling = treeA->child->lSibling;
		treeB->rSibling = treeA->child;
		treeA->child->lSibling->rSibling = treeB;
		treeA->child->lSibling = treeB;
	}

	treeA->child = treeB;
	treeA->degree++;
	return treeA;
}

// Assure there's no two trees with the same degree
void FFibonacciHeap::correctHeap()//TODO THIS IS WRONG!
{
	std::deque<FNodePtr> heapTrees;
	heapTrees.resize(128, nullptr);//a tree would never be this big, but this costs nothing...

	auto currTree = head->rSibling;
	FNodePtr aux;
	while (currTree != head)
	{
		aux = currTree;
		while (true) {
			if (heapTrees[aux->degree] == nullptr) {
				heapTrees[aux->degree] = aux;
				currTree = aux;
				break;
			}
			else {
				auto toErase = aux->degree;
				aux = linkTrees(aux, heapTrees[toErase]);
				heapTrees[toErase] = nullptr;
			}
		}
		currTree = currTree->rSibling;
	}

	//re-arrange the heap;
	head->lSibling = head = head->rSibling = head;
	min = head;
	for (auto & tree : heapTrees) {
		if (tree != nullptr) {
			FFibonacciHeap aux;
			auto auxHead = aux.getHead();
			tree->lSibling = tree->rSibling = auxHead;
			auxHead->lSibling = auxHead->rSibling = tree;
			merge(aux);
		}
	}
}

void FFibonacciHeap::insert(int key)
{
	FNodePtr node = new FNode;
	FFibonacciHeap newHeap;
	auto nHead = newHeap.getHead();
	
	initializeNode(node, key, nHead, nHead);
	nHead->rSibling = node;
	nHead->lSibling = node;
	newHeap.updateMin();

	merge(newHeap);
}

//min becomes head if the heap is empty
void FFibonacciHeap::updateMin()
{
	min = head;
	FNodePtr currPtr = min->rSibling;
	int minValue = INT32_MAX;

	while (currPtr != head) {
		if (currPtr->key < minValue) {
			minValue = currPtr->key;
			min = currPtr;
		}
		currPtr = currPtr->rSibling;
	}
}

void FFibonacciHeap::promoteToRoot(FNodePtr node)
{
	FNodePtr oldLSibling = node->lSibling, oldRSibling = node->rSibling, oldParent = node->parent;

	FFibonacciHeap newHeap;
	auto newHead = newHeap.getHead();
	newHead->rSibling = node;
	newHead->lSibling = node;
	node->parent = nullptr;
	node->loser = false;
	node->rSibling = newHead;
	node->lSibling = newHead;
	newHeap.updateMin();

	if (oldParent->child == node) {
		if (oldRSibling != node) {
			oldParent->child = oldRSibling;
			oldRSibling->lSibling = oldLSibling;
			oldLSibling->rSibling = oldRSibling;
		}
		else {
			oldParent->child = nullptr;
		}
		oldParent->degree--;
	}
	else {
		oldRSibling->lSibling = oldLSibling;
		oldLSibling->rSibling = oldRSibling;
	}

	merge(newHeap);
}

//Returns head if heap is empty
FNodePtr FFibonacciHeap::getMin() {	return min; }

//Merge two heaps in O(1), the second heap becomes a part of the first, ceasing to exist by its own
void FFibonacciHeap::merge(FFibonacciHeap & HeapB)
{
	FNodePtr heapBHead = HeapB.getHead();
	heapBHead->rSibling->lSibling = head->lSibling;
	heapBHead->lSibling->rSibling = head;
	head->lSibling->rSibling = heapBHead->rSibling;
	head->lSibling = heapBHead->lSibling;

	if (HeapB.getMin()->key < min->key) {
		min = HeapB.getMin();
	}

	// killing B ***************
	HeapB = FFibonacciHeap();
	//**************************
}

void FFibonacciHeap::extractMin()
{
	auto minChild = min->child;

	min->lSibling->rSibling = min->rSibling;
	min->rSibling->lSibling = min->lSibling;

	if (minChild != nullptr) {
		FFibonacciHeap newHeap;
		auto newHead = newHeap.getHead();
		newHead->rSibling = minChild;
		newHead->lSibling = minChild->lSibling;
		minChild->lSibling->rSibling = newHead;
		minChild->lSibling = newHead;

		auto curr = newHead->rSibling;
		while (curr != newHead) {
			curr->parent = nullptr;
			curr = curr->rSibling;
		}

		merge(newHeap);
	}

	min = head;
	correctHeap();
	updateMin();
}

void FFibonacciHeap::decreaseKey(FNodePtr node, unsigned int shift)
{
	node->key -= shift;
	auto parent = node->parent;
	if (parent != nullptr && node->key < parent->key) {
		promoteToRoot(node);
		while (parent->parent != nullptr) {
			if (!parent->loser) {
				parent->loser = true;
				break;
			}
			else {
				auto aux = parent->parent;
				promoteToRoot(parent);
				parent = aux;
			}
		}
	}
}

void FFibonacciHeap::extractNode(FNodePtr node)
{
	decreaseKey(node, INT32_MAX + node->key);
	extractMin();
}

FNodePtr FFibonacciHeap::getHead() { return head; }

void FFibonacciHeap::setHead(FNodePtr head) { this->head = head; }

void FFibonacciHeap::insertFromDeque(std::deque<int> & const elements)
{
	for (auto & element : elements) {
		insert(element);
	}
}

/* 
It doesn't print it by level or by any other sensible order. 
The trees are printed from left to right and from parent to child.
*/
void FFibonacciHeap::printTree(FNodePtr node) {
	auto currNode = node;
	do {
		std::cout << currNode->key << " ";
		if (currNode->child != nullptr) {
			printTree(currNode->child);
		}
		currNode = currNode->rSibling;
	} while (currNode != nullptr && currNode->parent != nullptr && currNode != node);
}

void FFibonacciHeap::printHeap()
{
	std::cout << "\nHeap: \n";
	auto currPtr = head->rSibling;
	while (currPtr != head) {
		std::cout << "Tree of degree: " << currPtr->degree << "\n\n";
		printTree(currPtr);
		std::cout << "\n\n";
		currPtr = currPtr->rSibling;
	}
	std::cout << "\n";
}
