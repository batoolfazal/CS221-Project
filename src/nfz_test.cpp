#include <iostream>
#include "../include/NFZ.h"

int main() {
    // Initialize global NFZ pointer if needed
    insertNFZ(10, 10);
    insertNFZ(12, 15);
    insertNFZ(20, 25);
    insertNFZ(5, 5);

    std::cout << "NFZ Test\n";
    int tests[][2] = {{10,10},{12,15},{20,25},{5,5},{7,7},{0,0}};
    int n = 6;
    for (int i = 0; i < n; i++) {
        int r = tests[i][0];
        int c = tests[i][1];
        std::cout << "(" << r << "," << c << "): " << (isNFZ(r,c) ? "NFZ" : "SAFE") << "\n";
    }

    removeNFZ(12, 15);
    std::cout << "After removal (12,15): " << (isNFZ(12,15) ? "NFZ" : "SAFE") << "\n";
    return 0;
}

