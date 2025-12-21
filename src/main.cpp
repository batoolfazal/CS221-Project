// Mission orchestrator: DFS -> BFS -> A* with NFZ and hazards, telemetry, report
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "CampusMap.h"
#include "GridMap.h"
#include "NFZ.h"
#include "hashmap.h"
#include "telemetry.h"
#include "sort_utils.h"
#include "astar.h"

// Global telemetry sink for A* tracePath (declared extern in astar.cpp)
TelemetryLog* gTelemetryPtr = 0;

static void suggestNearestValid(CampusMap& map, int& r, int& c) {
    if (map.isFree(r, c)) return;
    int bestR = -1, bestC = -1;
    for (int dist = 1; dist <= 3 && bestR == -1; dist++) {
        for (int dr = -dist; dr <= dist; dr++) {
            for (int dc = -dist; dc <= dist; dc++) {
                int nr = r + dr;
                int nc = c + dc;
                if (map.isInBounds(nr, nc) && map.isFree(nr, nc)) {
                    bestR = nr; bestC = nc; break;
                }
            }
            if (bestR != -1) break;
        }
    }
    if (bestR != -1) { r = bestR; c = bestC; }
}

static void promptPoint(const char* label, CampusMap& map, int& r, int& c) {
    std::cout << "Enter " << label << " row col: ";
    std::cin >> r >> c;
    if (!map.isInBounds(r, c) || !map.isFree(r, c)) {
        std::cout << "Invalid/blocked. Searching nearby...\n";
        suggestNearestValid(map, r, c);
        if (map.isFree(r, c)) {
            std::cout << "Using nearest free cell: (" << r << "," << c << ")\n";
        } else {
            std::cout << "No nearby free cell found. Using default (1,1).\n";
            r = 1; c = 1;
        }
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    // 1. Load grid
    CampusMap campus(80, 80);
    if (!campus.loadFromFile("Mapping/campus_map.txt")) {
        campus.buildMap();
        campus.saveToFile("Mapping/campus_map.txt");
    }

    // 2. NFZ DB (sample entries)
    insertNFZ(10, 35);
    insertNFZ(29, 55);
    insertNFZ(40, 55);

    // 3. Dynamic hazards
    generateRandomHazards(10, campus.getRows(), campus.getCols(), "dyn");

    // Waypoints
    int startR = 74, startC = 40;
    int goalR = 15, goalC = 34;
    promptPoint("START", campus, startR, startC);
    promptPoint("GOAL", campus, goalR, goalC);

    // Telemetry sink
    TelemetryLog telemetry;
    gTelemetryPtr = &telemetry;

    // 4. DFS pre-check
    std::cout << "Running DFS pre-check...\n";
    runDfs(campus, startR, startC);

    // 5. BFS pre-check
    std::cout << "Running BFS connectivity check...\n";
    bool bfsOk = bfsConnectivity(campus, startR, startC, goalR, goalC);
    std::cout << "BFS says path " << (bfsOk ? "exists" : "blocked") << "\n";

    // 6. A* pathfinding
    std::cout << "Running A*...\n";
    aStarSearch(campus, campus.getRows(), campus.getCols(), startR, startC, goalR, goalC);

    // 7. Re-route if new hazard appears
    std::cout << "Injecting new hazards and re-routing...\n";
    addHazard((startR + goalR) / 2, (startC + goalC) / 2, "mid_path_block");
    aStarSearch(campus, campus.getRows(), campus.getCols(), startR, startC, goalR, goalC);

    // 8. Telemetry captured via gTelemetryPtr

    // 9. Visualization placeholder
    std::cout << "Visualization: (placeholder)\n";

    // 10. Report generation with sorting
    const int MAX = 4096;
    TelemetryEntry arr[MAX];
    int n = telemetry.toArray(arr, MAX);
    if (n <= 16) {
        insertionSort(arr, n, cmpByRowCol);
    } else if (n <= 256) {
        quickSort(arr, n, cmpByRowCol);
    } else {
        mergeSort(arr, n, cmpByRowCol);
    }

    std::ofstream out("mission_summary.txt");
    if (out.is_open()) {
        out << "Mission Summary\n";
        out << "Total steps: " << n << "\n";
        out << "Path (sorted row/col):\n";
        for (int i = 0; i < n; i++) {
            out << "(" << arr[i].row << "," << arr[i].col << ")\n";
        }
        out.close();
        std::cout << "mission_summary.txt written.\n";
    } else {
        std::cout << "Failed to write mission_summary.txt\n";
    }

    return 0;
}
