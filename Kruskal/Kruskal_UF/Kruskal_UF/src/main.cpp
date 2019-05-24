#include "Kruskal.h"
#include "LoadInstance.h"

#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char** argv)
{
	start:
	std::cout << "Instance file: \n";
	std::string InstFile;
	std::getline(std::cin, InstFile);
	TInstData InstData = LoadInstance(InstFile);

	auto StartTime = std::chrono::high_resolution_clock::now();

	InstData = Kruskal(InstData);

	auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
	auto ElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime).count();

	for (auto Line : InstData) {
		for (auto Value : Line) {
			//std::cout << Value << " ";
		}
		//std::cout << std::endl;
	}
	std::cout << "\nTempo corrido: " << ElapsedMS << std::endl;

	std::cout << "Continue?(Y/n): ";
	std::string Cont;
	std::getline(std::cin, Cont);
	if (Cont != "n") goto start;
	system("pause");
	return 0;
}
