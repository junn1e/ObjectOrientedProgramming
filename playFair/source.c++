#include <iostream>
#include <unordered_set>
#include <algorithm>
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
	string alpha = "abcdefghijklmnopqrstuvwxyz";
	string cpKey = mKey + alpha; // 키와 알파벳 결합

	// 중복 제거를 위한 unordered_set 사용
	unordered_set<char> seen; 
	string uniqueKey;

	for (char ch : cpKey) {
		if (ch != mPair[2] && seen.find(ch) == seen.end()) {
			uniqueKey += ch;
			seen.insert(ch);
		}
	}

	// 5x5 테이블 생성
	for (int k = 0; k < 25; k++) {
		this->mTable[k] = uniqueKey[k];
	}
}

void Playfair::showTable() {
	// mTable에 있는 값들을 5 X 5 로 화면에 출력하시오.
	cout << "- - - - - - - table - - - - - - -" << endl;
	for (int i = 0; i < 25; i++) {
		if (mTable[i] == mPair[0]) {
			cout << mTable[i] << '/' << mPair[2];
		} else {
			cout << mTable[i];
		}
		cout << ((i % 5 == 4) ? '\n' : '\t'); // 5개마다 줄바꿈
	}
	cout << "- - - - - - - - - - - - - - - - -" << endl << endl;
}

void encryptByTable(char* table, char* key) {
	int index_one = -1, index_two = -1;
	
	// 첫 번째 문자(key[0])와 두 번째 문자(key[1])의 위치 찾기
	for (int i = 0; table[i] != '\0'; i++) {
		if (key[0] == table[i]) index_one = i;
		if (key[1] == table[i]) index_two = i;
	}

	// - 열이 같을 때
	if (index_one % 5 == index_two % 5) {
		// 각 문자를 1칸 아래로 이동
		index_one = (index_one + 5) % 25;
		index_two = (index_two + 5) % 25;
	}
	// - 행이 같을 때
	else if (index_one / 5 == index_two / 5) {
		// 각 문자를 1칸 오른쪽으로 이동
		index_one = (index_one + 1) % 5 + (index_one / 5) * 5;
		index_two = (index_two + 1) % 5 + (index_two / 5) * 5;
	}
	// - 모두 다를 때
	else {
		// 각 문자의 행은 그대로 두고 열만 바꿔서 이동
		int temp = index_one / 5 * 5 + index_two % 5;
		index_two = index_two / 5 * 5 + index_one % 5;
		index_one = temp;
	}

	key[0] = table[index_one];
	key[1] = table[index_two];
}

// 하는 일 : 테이블을 기반으로 입력된 문자열을 암호화 후 반환
string Playfair::makeEncryption(string mEncryption) {
	int index = -1;
	char key[2] = {'0', '0'};
	int marker;
	string result;
	// 문자열의 모든 글자에 대해 수행
	while ( mEncryption[++index] != '\0' )
	{
		// 알파벳이 아닐 경우 : 그냥 넘기기
		if(isalpha(mEncryption[index]) == 0) {
			result += mEncryption[index];
			continue;
		}

		// 첫번째 슬롯이 비어있을경우 -> 채우고 원래 자리는 일단 공백으로 냅두기
		if(key[0] == '0'){
			key[0] = mEncryption[index];
			marker = index;
			result += ' ';
			continue;
		}

		// 두번째 슬롯까지 채운 경우 -> 암호화 수행
		key[1] = mEncryption[index];

		key[0] = key[0] <= 'Z' ? key[0] += 32 : key[0];
		key[1] = key[1] <= 'Z' ? key[1] += 32 : key[1];

		
		encryptByTable(mTable, key);

		// 저장
		result[marker] = key[0];
		result += key[1];
		//cout << '[' << key[0] << ',' << key[1] << ']' << endl;
		key[0] = '0';
	}
	
	return result;
}



int main() {
	Playfair pfair("assasinator", "q/z");
	pfair.makeTable();
	pfair.showTable();
	string result = pfair.makeEncryption("Hello my name is jeong dong joon.");
	cout << result;
}