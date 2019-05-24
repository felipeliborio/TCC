#include "AlphaKruskalTCP.h"
#include "LoadInstance.h"
#include "LNS_TCP.h"

#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <filesystem>
#include <windows.h>

TEdges LoadEdges(TEdges & InstData);
std::unordered_set<int> LoadTerminals(TEdges & InstData);
FInstance GetInstance(std::string const & InstanceDir);
std::deque<std::string> ListDirectoryFiles(std::string Directory);
void RunInstance(std::string & InstanceDir);

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	/*
	std::cout << "Instance ";
	std::cout << "Time ";
	std::cout << "Size\n";
	std::string InputDirectory = "D:/dev/TCC/Instances";
	for (auto InstanceFile : ListDirectoryFiles(InputDirectory)) {
		RunInstance(InputDirectory + "/" + InstanceFile);
	}
	*/
	/**/
start:
	RunInstance((std::string)"D:/dev/TCC/Instances/steine20.txt");
	std::cout << "Continue?(Y/n): ";
	std::string Cont;
	std::getline(std::cin, Cont);
	if (Cont != "n") goto start;
	/**/
	return 0;
}

void RunInstance(std::string & InstanceDir) {
	FInstance Instance = GetInstance(InstanceDir);
	auto StartTime = std::chrono::high_resolution_clock::now();
	//Instance.MaxL = (Instance.VertexCount - Instance.TerminalCount) / 3;
	//Instance.MaxR = (Instance.VertexCount - Instance.TerminalCount) / 4;

	TEdges Solution = Kruskal(Instance.Edges, Instance.Terminals, Instance.VertexCount, INT32_MAX, INT32_MAX, (float) 0.2);
	CutInstance(Solution, Instance.Terminals);
	Solution = LNS_TCP(Instance, Solution);

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	auto ElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	/*
	int Length = 0;
	for (auto Line : Solution) {
		for (auto Value : Line) {
			std::cout << Value << " ";
		}
		Length += Line[2];
		std::cout << std::endl;
	}
	*/
	int Length = GraphLength(Solution);
	std::cout << "" << InstanceDir.substr(InstanceDir.size() - 12, 12) << " ";
	std::cout << "" << ElapsedMS << " ";
	std::cout << "" << Length << std::endl;
}

TEdges LoadEdges(TEdges & InstData)
{
	TEdges Output;
	for (int i = 1; i <= InstData[0][1]; i++) {
		Output.push_back(InstData[i]);
	}
	return Output;
}

std::unordered_set<int> LoadTerminals(TEdges & InstData)
{
	std::unordered_set<int> Output;
	for (int i = InstData[0][1] + 2; i < InstData.size(); i++) {
		for (int Terminal : InstData[i]) {
			Output.insert(Terminal);
		}
	}
	return Output;
}

FInstance GetInstance(std::string const & InstanceDir)
{
	FInstance Instance;
	TEdges InstData = LoadInstance(InstanceDir);
	Instance.VertexCount = InstData[0][0];
	Instance.EdgeCount = InstData[0][1];
	Instance.Edges = LoadEdges(InstData);
	Instance.TerminalCount = InstData[Instance.EdgeCount + 1][0];
	Instance.Terminals = LoadTerminals(InstData);
	return Instance;
}

std::deque<std::string> ListDirectoryFiles(std::string Directory) {
	std::deque<std::string> Output;
	std::string search_path = Directory + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				Output.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return Output;
}
