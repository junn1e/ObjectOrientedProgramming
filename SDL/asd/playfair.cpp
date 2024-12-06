#include "Playfair.h"

Playfair::Playfair(string mKey, string mPair) {
    this->mKey = mKey;
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
        }
        else {
            cout << mTable[i];
        }
        cout << ((i % 5 == 4) ? '\n' : '\t');
    }
}

void encryptByTable(char* table, char* key) {
    int index_one = -1, index_two = -1;

    for (int i = 0; i < 25; i++) {
        if (key[0] == table[i]) index_one = i;
        if (key[1] == table[i]) index_two = i;
    }

    int row_one = index_one / 5, col_one = index_one % 5;
    int row_two = index_two / 5, col_two = index_two % 5;

    if (row_one == row_two) {
        index_one = row_one * 5 + (col_one + 1) % 5;
        index_two = row_two * 5 + (col_two + 1) % 5;
    }
    else if (col_one == col_two) {
        index_one = ((row_one + 1) % 5) * 5 + col_one;
        index_two = ((row_two + 1) % 5) * 5 + col_two;
    }
    else {
        index_one = row_one * 5 + col_two;
        index_two = row_two * 5 + col_one;
    }

    key[0] = table[index_one];
    key[1] = table[index_two];

    string a;

    a += key[0];
    a += key[1];
    //cout << a;
}

string Playfair::makeEncryption(string mEncryption) {
    int index = -1;
    char key[2] = { '0', '0' };
    int marker;
    string result;

    while (mEncryption[++index] != '\0') {
        if (isalpha(mEncryption[index]) == 0) {
            result += mEncryption[index];
            continue;
        }

        if (key[0] == '0') {
            key[0] = mEncryption[index];
            continue;
        }

        key[1] = mEncryption[index];

        if (key[0] == key[1]) {
            key[1] = 'x';
        }
        if (key[0] == mPair[0]) key[0] = mPair[2];
        else if (key[0] == mPair[2])key[0] = mPair[0];
        if (key[1] == mPair[0]) key[1] = mPair[2];
        else if (key[1] == mPair[2])key[1] = mPair[0];
        encryptByTable(mTable, key);

        
        result += key;
        string a;
        a += key[0];
        a += key[1];
        cout << a;
        key[0] = '0';
    }
    if (key[0] != '0') {
        key[1] = 'x';
        if (key[0] == mPair[0]) key[0] = mPair[2];
        else if (key[0] == mPair[2] )key[0] = mPair[0];
        if (key[1] == mPair[0]) key[1] = mPair[2];
        else if (key[1] == mPair[2] )key[1] = mPair[0];
        encryptByTable(mTable, key);
        
        string a;
        a += key[0];
        a += key[1];
        cout << a;
        result += key;
    }
    return result;
}
