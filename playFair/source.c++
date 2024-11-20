#include <iostream>
using namespace std;

class Playfair {
	char mTable[25];
	string mPair;
	string mKey;
public:
	Playfair(string mKey, string mPair);
	void makeTable();
	void showTable();
	string makeEncryption(string mEncryption);
};

Playfair::Playfair(string mKey, string mPair) {
	this->mKey = mKey;
	this->mPair = mPair;
}

void Playfair::makeTable() {
	// mKey와 mPair를 이용하여 mTable을 만드세요. (5 X 5 로 표현하시오)
}

void Playfair::showTable() {
	// mTable에 있는 값들을 5 X 5 로 화면에 출력하시오.
}

string Playfair::makeEncryption(string mEncryption) {
	return NULL;
}


int main() {
	Playfair pfair("assasinator", "q/z");
	pfair.makeTable();
}