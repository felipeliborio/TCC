#include "LoadInstance.h"

#include <fstream>
#include <sstream>
#include <string>

using TInstText = std::deque<std::deque<std::string>>;

//*****************************************************************************
//splits a string based on a delimiter
template<typename Out>
void Split(const std::string &s, char delim, Out result)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
	return;
}

std::deque<std::string> Split(const std::string &s, char delim)
{
	std::deque<std::string> elems;
	Split(s, delim, std::back_inserter(elems));
	return elems;
}
//*****************************************************************************

void RemoveEmptyElements(std::deque<std::deque<std::string>> &Input)
{
	int Delta;
	std::deque<std::string>::iterator Iterator;
	for (int Line = 0; Line < Input.size(); Line++) {
		Delta = 0;
		for (int Word = 0; Word < Input[Line].size(); Word++) {
			if (Input[Line][Word - Delta] == " " || Input[Line][Word - Delta].length() < 1) {
				Iterator = Input[Line].begin();
				advance(Iterator, Word - Delta);
				Input[Line].erase(Iterator);
				Delta++;
			}
		}
	}
	return;
}

TInstText GetFileLines(const std::string FileDirectory)
{
	std::ifstream File(FileDirectory, std::ios::in);
	TInstText Input = {};
	std::string Line;
	while (std::getline(File, Line)) {
		Input.push_back({ Split(Line, ' ') });
	}
	File.close();
	RemoveEmptyElements(Input);
	return Input;
}

TEdges TextToInt(const std::deque<std::deque<std::string>> &Input)
{
	std::deque<std::deque<int>> Output;
	for (auto Line : Input) {
		Output.push_back({});
		for (auto Word : Line) {
			Output[Output.size() - 1].push_back(stoi(Word));
		}
	}
	return Output;
}

TEdges LoadInstance(const std::string FileDirectory)
{
	TInstText FileLines = GetFileLines(FileDirectory);
	return TextToInt(FileLines);
}


