#include "Playfair.h"

Playfair::Playfair(string mKey, string mPair) {
    this->mKey = mKey;
    if (mPair[0] > mPair[2]) {
        mPair[1] = mPair[2];
        mPair[2] = mPair[0];
        mPair[0] = mPair[1];
        mPair[1] = '/';
    }
    this->mPair = mPair;
}

void Playfair::makeTable() {
    mKey += "abcdefghijklmnopqrstuvwxyz";
    string result;
    unordered_set<char> keySet;

    for (char c : mKey) {
        if (c != mPair[2] && keySet.find(c) == keySet.end()) {
            result += c;
            keySet.insert(c);
        }
    }

    for (int k = 0; k < 25; k++) {
        this->mTable[k] = result[k];
    }
}

void Playfair::showTable() {
    for (int i = 0; i < 25; i++) {
        if (mTable[i] == mPair[0]) {
            cout << mTable[i] << '/' << mPair[2];
        } else {
            cout << mTable[i];
        }
        cout << ((i % 5 == 4) ? '\n' : '\t');
    }
}

void encryptByTable(char* table, char* key) {
    int index_one = -1, index_two = -1;
    if (key[0] == key[1]) return;

    for (int i = 0; table[i] != '\0'; i++) {
        if (key[0] == table[i]) index_one = i;
        if (key[1] == table[i]) index_two = i;
    }

    if (index_one % 5 == index_two % 5) {
        index_one = (index_one + 5) % 25;
        index_two = (index_two + 5) % 25;
    } else if (index_one / 5 == index_two / 5) {
        index_one = (index_one + 1) % 5 + (index_one / 5) * 5;
        index_two = (index_two + 1) % 5 + (index_two / 5) * 5;
    } else {
        int temp = index_one / 5 * 5 + index_two % 5;
        index_two = index_two / 5 * 5 + index_one % 5;
        index_one = temp;
    }

    key[0] = table[index_one];
    key[1] = table[index_two];
}

string Playfair::makeEncryption(string mEncryption) {
    int index = -1;
    char key[2] = {'0', '0'};
    int marker;
    string result;

    while (mEncryption[++index] != '\0') {
        if (isalpha(mEncryption[index]) == 0) {
            result += mEncryption[index];
            continue;
        }

        if (key[0] == '0') {
            key[0] = mEncryption[index];
            marker = index;
            result += ' ';
            continue;
        }

        key[1] = mEncryption[index];
        bool isUpper[2] = {
            static_cast<bool>(isupper(key[0])),
            static_cast<bool>(isupper(key[1]))
        };
        char lowerKey[2] = {
            static_cast<char>(tolower(key[0])),
            static_cast<char>(tolower(key[1]))
        };

        encryptByTable(mTable, lowerKey);

        if (key[0] == mPair[0]) key[0] = mPair[2];
        if (key[1] == mPair[0]) key[1] = mPair[2];

        result[marker] = isUpper[0] ? toupper(lowerKey[0]) : lowerKey[0];
        result += isUpper[1] ? toupper(lowerKey[1]) : lowerKey[1];

        key[0] = '0';
    }
    return result;
}
