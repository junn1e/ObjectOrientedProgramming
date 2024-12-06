#include "Playfair.h"

int main() {
    Playfair pfair("assasinator", "h/c");
    pfair.makeTable();
    pfair.showTable();
    string result = pfair.makeEncryption("Hello my name is Jeong Dong Joon.");
    cout << result;
    return 0;
}