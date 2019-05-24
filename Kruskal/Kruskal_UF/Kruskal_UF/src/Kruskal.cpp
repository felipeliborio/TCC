#include "Kruskal.h"
#include <algorithm>
#include <iostream>

int Find(int P, std::deque<int> &Id)
{
	if (Id[P] == -1 || Id[P] == P) return P;
	return Id[P] = Find(Id[P], Id);
}

void Union(int P, int Q, std::deque<int> &Id, std::deque<int> &Size)
{
	P = Find(P, Id);
	Q = Find(Q, Id);
	if (P != Q) {
		if (Size[P] < Size[Q]) std::swap(P, Q);
		Id[Q] = P;
		Size[P] += Size[Q];
	}
}

bool Compare(std::deque<int> const & I, std::deque<int> const & J) { return I[2] < J[2]; }

TInstData Kruskal(TInstData InstData) {
	TInstData Output;
	Output.push_back(InstData[0]);
	InstData.pop_front();
	InstData.resize(Output[0][1]);
	std::sort(InstData.begin(), InstData.end(), Compare);
	std::deque<bool> MST;
	MST.resize(InstData.size(), false);

	std::deque<int> Id;
	Id.resize(Output[0][0] + 1, -1);

	std::deque<int> Size;
	Size.resize(Output[0][0] + 1, 1);

	for (int E = 0; E < InstData.size(); E++) {
		if (Find(InstData[E][0], Id) == Find(InstData[E][1], Id)) continue;
		Union(InstData[E][0], InstData[E][1], Id, Size);
		MST[E] = true;
	}

	for (int I = 0; I < MST.size(); I++) {
		if (MST[I]) Output.push_back(InstData[I]);
	}
	Output.pop_front();

	int s = 0;
	for (auto & e : Output) {
		s += e[2];
	}

	std::cout << "length: " << s << "\n";

	return Output;
}

