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
	string makeDecryption(string mEncryption);
};

Playfair::Playfair(string mKey, string mPair) {
	
	this->mKey = mKey;
	// mPair 오름차순인가 내림차순으로 정리
	if (mPair[0] > mPair[2]) {
		mPair[1] = mPair[2];
		mPair[2] = mPair[0];
		mPair[0] = mPair[1];
		mPair[1] = '/';
	}
	this->mPair = mPair;
}

// 테이블 생성 함수: Playfair 암호화에 필요한 5x5 테이블 생성
void Playfair::makeTable() {
	// 키 뒤에 알파벳 붙이기
	mKey += "abcdefghijklmnopqrstuvwxyz";

	// 결과 저장용 문자열
	string result;
	
	// 중복 체크용 문자집합 (unordered_set)
	unordered_set<char> keySet;

	// 테이블 생성
	for (char c : mKey) {
		// keySet.find(c) : 중복이 아닐 시 keySet.end() 반환
		if (c != mPair[2] && keySet.find(c) == keySet.end()) {
			result += c;
			keySet.insert(c);
		}
	}

	// 5. 5x5 테이블 채우기
	for (int k = 0; k < 25; k++) {
		this->mTable[k] = result[k];
	}
}


void Playfair::showTable() {
	// mTable에 있는 값들을 5 X 5 로 화면에 출력하시오.
	for (int i = 0; i < 25; i++) {
		if (mTable[i] == mPair[0]) {
			cout << mTable[i] << '/' << mPair[2];
		} else {
			cout << mTable[i];
		}
		cout << ((i % 5 == 4) ? '\n' : '\t');
	}
}

// 하는 일 : 키 값 char[2]를 테이블을 기반으로 변환
void encryptByTable(char* table, char* key) {
	int index_one = -1, index_two = -1;
	
	// - 문자가 같을 때
	if (key[0] == key[1]) return;

	// 첫 번째 문자(key[0])와 두 번째 문자(key[1])의 인덱스 찾기
	for (int i = 0; table[i] != '\0'; i++) {
		if (key[0] == table[i]) index_one = i;
		if (key[1] == table[i]) index_two = i;
	}

	// -- 암호화 코드 (중요) --
	// - 열이 같을 때
	if (index_one % 5 == index_two % 5) {
		// 1칸 아래로 이동
		index_one = (index_one + 5) % 25;
		index_two = (index_two + 5) % 25;
	}
	// - 행이 같을 때
	else if (index_one / 5 == index_two / 5) {
		// 1칸 오른쪽으로 이동
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

	// 쌍 인덱스 처리는 이 함수 끝난뒤에 함

	// 결과 대입
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

		// 첫번째 슬롯이 비어있을경우 -> 채우고 원래 자리는 marker로 표시
		if(key[0] == '0'){
			key[0] = mEncryption[index];
			marker = index;
			result += ' ';
			continue;
		}

		// 두번째 슬롯까지 채운 경우 -> 암호화 수행
		key[1] = mEncryption[index];

		//   isUpper : 암호화할 문자열의 대소문자 보존용 리스트
    bool isUpper[2] = {
        static_cast<bool>(isupper(key[0])),
        static_cast<bool>(isupper(key[1]))
    };
		//   lowerKey : 소문자로 바꾼 키 쌍
    char lowerKey[2] = {
        static_cast<char>(tolower(key[0])),
        static_cast<char>(tolower(key[1]))
    };

    // 암호화 수행
    encryptByTable(mTable, lowerKey);

		// 쌍 인덱스 처리
		if( key[0] == mPair[0] ) key[0] = mPair[2];
		if( key[1] == mPair[0] ) key[1] = mPair[2];

    // 결과 저장
    result[marker] = isUpper[0] ? toupper(lowerKey[0]) : lowerKey[0];
    result        += isUpper[1] ? toupper(lowerKey[1]) : lowerKey[1];

    // 슬롯 초기화
    key[0] = '0';
	}
	return result;
}

int main() {
	Playfair pfair("assasinator", "h/c");
	pfair.makeTable();
	pfair.showTable();
	string result = pfair.makeEncryption("Hello my name is Jeong Dong Joon.");
	cout << result;
}