/*
MIT License

Copyright (c) <2019> <Felipe Lib�rio>

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

#include "definitions.h"
#include "read_instance.h"
#include "binary_heap.h"
#include "FPrim.h"
#include "FLNS.h"
#include <experimental/filesystem>
#include <windows.h>

namespace fs = std::filesystem;

int main() {
	srand((int) time(NULL));

	
	std::string inputDirectory = "D:/dev/TCC/__Instances/";
	std::string ph = "instance     ";
	std::cout << ph << "\tmin\tmax\tavg\tmedian\ttime\tto best\tl min\tl max\tr min\tr max\tsolved\n";
	for (const auto & file : fs::directory_iterator(inputDirectory)) {
		std::stringstream ss;
		ss << file.path();
		std::string dir = ss.str();
		dir.erase(dir.begin());
		dir.pop_back();
		auto instData = readInstanceData(dir);
		if (instData.size() == 0) {
			std::cout << "Coudn't load instance.\n";
			return 0;
		}

		FLNS LNS(instData);
		std::string instanceName = dir.substr(inputDirectory.size(), dir.size() - inputDirectory.size() - 4);

		//LNS.run();
		//LNS.printLastResult(false, instanceName);

		if (instanceName.size() < ph.size()) {
			instanceName.resize(ph.size());
		}
		LNS.runBattery(10, instanceName);
	}

	system("pause");
	return 0;
}
