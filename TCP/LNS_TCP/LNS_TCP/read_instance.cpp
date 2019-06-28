#include "read_instance.h"

std::vector<std::vector<int>> readInstanceData(std::string & dir)
{
	std::vector<std::vector<int>> output;
	std::ifstream infile(dir);

	std::string line;
	std::vector<int> aux;
	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		int e;
		while (iss >> e) {
			aux.push_back(e);
		}
		output.push_back(aux);
		aux.clear();
	}

	return output;
}
