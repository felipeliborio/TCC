#include "AlphaKruskal.h"
#include <algorithm>
#include <stdlib.h>

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
	
	int DSize = InstData.size();
	int R;
	for (int E = 0; E < DSize; E++) {

		int Alpha = int((DSize - E) * 0.2);
		if (Alpha > 0) {
			R = E + (rand() % Alpha);
		}
		else {
			R = E;
		}

		if (Find(InstData[R][0], Id) == Find(InstData[R][1], Id)) {
			std::swap(InstData[E], InstData[R]);
			std::swap(MST[E], MST[R]);
			continue;
		}
		Union(InstData[R][0], InstData[R][1], Id, Size);
		MST[R] = true;
		std::swap(InstData[E], InstData[R]);
		std::swap(MST[E], MST[R]);
	}

	for (int I = 0; I < MST.size(); I++) {
		if (MST[I]) Output.push_back(InstData[I]);
	}
	Output.pop_front();
	std::sort(Output.begin(), Output.end());
	return Output;
}

