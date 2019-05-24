#pragma once

#include <deque>
#include <iostream>


struct FNode {
	int degree;
	int key;
	bool loser;

	FNode * parent;
	FNode * lSibling;
	FNode * rSibling;
	FNode * child;
};

typedef FNode* FNodePtr;

class FFibonacciHeap
{
private:
	FNodePtr head = new FNode;
	FNodePtr min = new FNode;

	void FFibonacciHeap::initializeNode(FNodePtr node, int key, FNodePtr lSib = nullptr, FNodePtr rSib = nullptr,
		FNodePtr child = nullptr, FNodePtr parent = nullptr, int degree = 0, bool loser = false);
	static FNodePtr linkTrees(FNodePtr TreeA, FNodePtr TreeB);
	void correctHeap();
	void updateMin();
	void FFibonacciHeap::printTree(FNodePtr node);
	void promoteToRoot(FNodePtr node);

public:
	FFibonacciHeap(std::deque<int> & const elements = std::deque<int>());
	~FFibonacciHeap();

	FNodePtr getHead();
	void setHead(FNodePtr head);

	void insert(int key);
	FNodePtr getMin();
	void merge(FFibonacciHeap & HeapB);
	void extractMin();
	void decreaseKey(FNodePtr node, unsigned int shift);
	void extractNode(FNodePtr node);

	void insertFromDeque(std::deque<int> & const elements);

	void printHeap();
};
