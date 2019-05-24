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

#include "difinitions.h"
#include "read_instance.h"
#include "binary_heap.h"
#include "FPrim.h"



int main() {
	auto instData = readInstanceData(std::string("D:/dev/TCC/Instances/steine20.txt"));

	std::cout << "\n\n\n\n\n";

	if (instData.size() == 0) {
		std::cout << "Coudn't load instance.\n";
		return 0;
	}
	FPrim prim(instData);
	prim.graph.simplifyInstance();
	auto k = prim.run(prim.graph.terminals[3], 0.0);
	prim.printLastResult(true);
	prim.printLastResult(false);

	system("pause");

	return 0;
}

