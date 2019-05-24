#pragma once
#include <iostream>
#include <cmath>
#include <queue>
#include <stack>

struct FNode {
	int data;
	int degree;
	FNode * parent;
	FNode * child;
	FNode * sibling;
};

typedef FNode * FNodePtr;

class FBinomialHeap {
private:
	FNodePtr head;

	void initializeNode(FNodePtr node, int data, int degree);
	static void linkBinomialTrees(FNodePtr treeA, FNodePtr treeB);

public:
	FBinomialHeap();

	FNodePtr findMinimum();
	void insert(int data);
	void printHeap();
	FNodePtr getHead();
	void setHead(FNodePtr head);
	void merge(FBinomialHeap HeapA);
	FNodePtr deleteMin();
	void buildFromDeque(std::deque<int> elements);


	void createSampleHeap1() {
		// B0
		FNodePtr node1 = new FNode;
		initializeNode(node1, 5, 0);
		head = node1;

		// B1
		FNodePtr node2 = new FNode;
		initializeNode(node2, 17, 1);
		FNodePtr node3 = new FNode;
		initializeNode(node3, 27, 0);
		node2->child = node3;
		node3->parent = node2;

		// link B0 and B1
		node1->sibling = node2;

		// B3
		FNodePtr node4 = new FNode;
		initializeNode(node4, 12, 3);
		FNodePtr node5 = new FNode;
		initializeNode(node5, 18, 2);
		FNodePtr node6 = new FNode;
		initializeNode(node6, 16, 1);
		FNodePtr node7 = new FNode;
		initializeNode(node7, 15, 0);
		FNodePtr node8 = new FNode;
		initializeNode(node8, 23, 1);
		FNodePtr node9 = new FNode;
		initializeNode(node9, 30, 0);
		FNodePtr node10 = new FNode;
		initializeNode(node10, 22, 0);
		FNodePtr node11 = new FNode;
		initializeNode(node11, 25, 0);
		node4->child = node5;
		node5->parent = node4;
		node6->parent = node4;
		node7->parent = node4;
		node5->child = node8;
		node5->sibling = node6;
		node6->child = node10;
		node6->sibling = node7;
		node8->parent = node5;
		node9->parent = node5;
		node10->parent = node6;
		node8->child = node11;
		node8->sibling = node9;
		node11->parent = node8;

		// link B1 and B3
		node2->sibling = node4;
	}

	// create sample heap (given in figure Fig 10 (a))
	void createSampleHeap2() {
		// B0
		FNodePtr node1 = new FNode;
		initializeNode(node1, 5, 0);
		head = node1;

		// B2
		FNodePtr node2 = new FNode;
		initializeNode(node2, 6, 2);
		FNodePtr node3 = new FNode;
		initializeNode(node3, 12, 1);
		FNodePtr node4 = new FNode;
		initializeNode(node4, 34, 0);
		FNodePtr node5 = new FNode;
		initializeNode(node5, 33, 0);
		node2->child = node3;
		node3->parent = node2;
		node4->parent = node2;
		node3->child = node5;
		node3->sibling = node4;
		node5->parent = node3;

		// link B0 and B1
		node1->sibling = node2;

		// B3
		FNodePtr node6 = new FNode;
		initializeNode(node6, 1, 3);
		FNodePtr node7 = new FNode;
		initializeNode(node7, 2, 2);
		FNodePtr node8 = new FNode;
		initializeNode(node8, 12, 1);
		FNodePtr node9 = new FNode;
		initializeNode(node9, 6, 0);
		FNodePtr node10 = new FNode;
		initializeNode(node10, 4, 1);
		FNodePtr node11 = new FNode;
		initializeNode(node11, 13, 0);
		FNodePtr node12 = new FNode;
		initializeNode(node12, 18, 0);
		FNodePtr node13 = new FNode;
		initializeNode(node13, 7, 0);
		node6->child = node7;
		node7->parent = node6;
		node8->parent = node6;
		node7->parent = node6;
		node7->child = node10;
		node7->sibling = node8;
		node8->child = node12;
		node8->sibling = node9;
		node10->parent = node7;
		node11->parent = node7;
		node12->parent = node8;
		node10->child = node13;
		node10->sibling = node11;
		node13->parent = node10;

		// link B1 and B3
		node2->sibling = node6;
	}

	// crete sample heap (given in figure 10 (b))
	void createSampleHeap3() {
		// B1
		FNodePtr node1 = new FNode;
		initializeNode(node1, 29, 1);
		FNodePtr node2 = new FNode;
		initializeNode(node2, 33, 0);
		node1->child = node2;
		node2->parent = node1;
		head = node1;

		// B2
		FNodePtr node3 = new FNode;
		initializeNode(node3, 21, 2);
		FNodePtr node4 = new FNode;
		initializeNode(node4, 23, 1);
		FNodePtr node5 = new FNode;
		initializeNode(node5, 78, 0);
		FNodePtr node6 = new FNode;
		initializeNode(node6, 50, 0);
		node3->child = node4;
		node4->parent = node3;
		node5->parent = node3;
		node4->child = node6;
		node4->sibling = node5;
		node6->parent = node4;

		// link B1 and B2
		node1->sibling = node3;

	}
};
