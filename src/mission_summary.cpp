#include <iostream>
#include <fstream>
#include "telemetry.h"
#include "sort_utils.h"

// Simple demo: build a telemetry log, export to array, sort, and write a summary.

static void writeSummary(const char* filename, TelemetryEntry* arr, int n) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "Failed to open summary file\n";
        return;
    }
    out << "Mission Summary\n";
    out << "Total steps: " << n << "\n";
    out << "Sorted telemetry (row,col):\n";
    for (int i = 0; i < n; i++) {
        out << "(" << arr[i].row << "," << arr[i].col << ")\n";
    }
    out.close();
    std::cout << "Summary written to " << filename << std::endl;
}

int main() {
    TelemetryLog log;
    // Demo data (would come from A* tracePath logging)
    log.logStep(3, 4);
    log.logStep(1, 2);
    log.logStep(5, 6);
    log.logStep(2, 2);

    const int MAX = 128;
    TelemetryEntry entries[MAX];
    int n = log.toArray(entries, MAX);

    // Choose sort based on size
    if (n <= 16) {
        insertionSort(entries, n, cmpByRowCol);
    } else if (n <= 256) {
        quickSort(entries, n, cmpByRowCol);
    } else {
        mergeSort(entries, n, cmpByRowCol);
    }

    writeSummary("mission_summary.txt", entries, n);
    return 0;
}

