#include "AlphaKruskal.h"
#include "LoadInstance.h"
#include "LNS_Steiner.h"

#include <iostream>
#include <string>
#include <filesystem>


TEdges LoadEdges(TEdges & InstData);
std::unordered_set<int> LoadTerminals(TEdges & InstData);
FInstance GetInstance(std::string & const InstanceDir);

int main(int argc, char** argv)
{
	start:
	std::cout << "Instance file: \n";
	std::string InstanceDir;
	std::getline(std::cin, InstanceDir);

	FInstance Instance = GetInstance(InstanceDir);

	auto StartTime = std::chrono::high_resolution_clock::now();

	TEdges Solution = Kruskal(Instance.Edges, Instance.VertexCount, (float) 0.2);
	CutInstance(Solution, Instance.Terminals);
	Solution = LNS_Steiner(Instance, Solution);

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	auto ElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	int Length = 0;
	for (auto Line : Solution) {
		for (auto Value : Line) {
			std::cout << Value << " ";
		}
		Length += Line[2];
		std::cout << std::endl;
	}

	std::cout << "\nTempo corrido: " << ElapsedMS << std::endl;
	std::cout << "\nTamanho: " << Length << std::endl;

	std::cout << "Continue?(Y/n): ";
	std::string Cont;
	std::getline(std::cin, Cont);
	if (Cont != "n") goto start;
	//system("pause");
	return 0;
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

FInstance GetInstance(std::string & const InstanceDir)
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