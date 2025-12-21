#include "astar.h"
#include "../Mapping/CampusMap.h"
#include <iostream>

int main() {
    int rows = 10, cols = 10;

    // Create CampusMap
    CampusMap map(rows, cols);

    // Mark some obstacles
    map.setCell(3, 3, 1);
    map.setCell(3, 4, 1);
    map.setCell(4, 3, 1);

    // Mark a no-fly zone
    map.setCell(5, 5, 2);

    // Mark hazards
    map.setHazard(6, 6, true);

    int srcRow = 0, srcCol = 0;
    int destRow = 9, destCol = 9;

    std::cout << "Running A* from (" << srcRow << "," << srcCol 
              << ") to (" << destRow << "," << destCol << ")\n";

    aStarSearch(map, rows, cols, srcRow, srcCol, destRow, destCol);

    return 0;
}

