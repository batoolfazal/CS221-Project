#include <iostream>
#include "../Mapping/CampusMap.h"
#include "../include/NFZ.h"
#include "../include/astar.h"

int main() {
    std::cout << "=== Safety Collision Check Test ===\n";

    // Initialize map and build regions
    CampusMap map(80, 80);
    map.buildMap();

    // Insert NFZs (dynamic) near a potential corridor
    insertNFZ(70, 40);
    insertNFZ(69, 40);
    insertNFZ(68, 40);
    insertNFZ(40, 40); // mid-map blocker

    // Verify NFZ insertion
    std::cout << "NFZ at (70,40): " << (isNFZ(70,40) ? "YES" : "NO") << "\n";
    std::cout << "NFZ at (40,40): " << (isNFZ(40,40) ? "YES" : "NO") << "\n";
    std::cout << "NFZ at (10,10): " << (isNFZ(10,10) ? "YES" : "NO") << "\n";

    int startR = 74, startC = 40;
    int goalR = 15, goalC = 34;

    std::cout << "\nRunning A* from (" << startR << "," << startC << ") to ("
              << goalR << "," << goalC << ")\n";
    aStarSearch(map, map.getRows(), map.getCols(), startR, startC, goalR, goalC);

    std::cout << "\n(Note: Path printed above should contain no coordinates where isNFZ(...) is true.)\n";
    return 0;
}

