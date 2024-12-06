#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <string>
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

void encryptByTable(char* table, char* key);

#endif // PLAYFAIR_H
