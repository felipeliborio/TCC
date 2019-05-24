/*
MIT License

Copyright(c) 2018 Jiacheng Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FIB_HEAP_H
#define _FIB_HEAP_H

#include <cmath>

namespace fib_heap {


	struct FibHeapNode
	{
		int key; // assume the element is int...
		FibHeapNode* left;
		FibHeapNode* right;
		FibHeapNode* parent;
		FibHeapNode* child;
		int degree;
		bool mark;
	};


	class FibHeap {
	public:
		FibHeapNode* m_minNode;
		int m_numOfNodes;

		FibHeap() {  // initialize a new and empty Fib Heap
			m_minNode = nullptr;
			m_numOfNodes = 0;
		}

		~FibHeap() {
			_clear(m_minNode);
		}

		/* Insert a node with key value new_key
		   and return the inserted node*/
		FibHeapNode* insert(int newKey);

		/* Merge current heap with another*/
		void merge(FibHeap &another);

		/* Return the key of the minimum node*/
		int  extract_min();

		/* Decrease the key of node x to newKey*/
		void decrease_key(FibHeapNode* x, int newKey);

		/*Delete a specified node*/
		void delete_node(FibHeapNode* x);

	private:
		static const int m_minimumKey;
		FibHeapNode* _create_node(int newKey);
		void _insert_node(FibHeapNode* newNode);
		void _remove_from_circular_list(FibHeapNode* x);
		FibHeapNode* _merge(FibHeapNode* a, FibHeapNode* b);
		void _make_child(FibHeapNode *child, FibHeapNode *parent);
		void _consolidate();
		void _unparent_all(FibHeapNode* x);
		FibHeapNode* _extract_min_node();
		void _decrease_key(FibHeapNode* x, int newKey);
		void _cut(FibHeapNode* x, FibHeapNode* y);
		void _cascading_cut(FibHeapNode* y);
		void _clear(FibHeapNode* x);
	};


}

#endif
