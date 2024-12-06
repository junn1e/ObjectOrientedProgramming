#include "Playfair.h"

int main() {
    Playfair pfair("goodboy", "b/y");
    pfair.makeTable();
    pfair.showTable();
    string result = pfair.makeEncryption("ooayy");
    //cout << result;
    return 0;
}