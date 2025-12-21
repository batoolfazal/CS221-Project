#include "HazardDetector.h"
#include <iostream>

// Cross-platform sleep
#ifdef _WIN32
#include <windows.h>
#define sleep_seconds(s) Sleep((s) * 1000)
#else
#include <unistd.h>
#define sleep_seconds(s) sleep(s)
#endif

using namespace std;

int main() {
    cout << "\n";
    cout << "============================================\n";
    cout << "   HAZARD DETECTION SYSTEM - DEMO\n";
    cout << "============================================\n\n";
    
    // Initialize detector for 60x40 map
    HazardDetector detector(60, 40);
    
    cout << "\n=== ADDING DYNAMIC HAZARDS ===" << endl;
    
    // Add various hazards
    detector.addHazard(25, 15, "BIRD", "Flock of birds crossing", 15, 3);
    detector.addHazard(40, 20, "DRONE", "Unauthorized drone detected", 30, 4);
    detector.addHazard(10, 30, "WEATHER", "Storm system approaching", 60, 2);
    detector.addHazard(50, 10, "DEBRIS", "Falling debris from building", 20, 5);
    detector.addHazard(35, 25, "AIRCRAFT", "Small aircraft in vicinity", 45, 5);
    
    // Display status
    detector.displayStatus();
    detector.displayAllHazards();
    
    // Test path safety
    cout << "\n=== PATH SAFETY TEST ===" << endl;
    
    vector<pair<int, int> > path1;
    path1.push_back(make_pair(20, 10));
    path1.push_back(make_pair(25, 15));
    path1.push_back(make_pair(30, 20));
    
    vector<pair<int, int> > path2;
    path2.push_back(make_pair(5, 5));
    path2.push_back(make_pair(10, 10));
    path2.push_back(make_pair(15, 15));
    
    cout << "\nChecking Path 1: ";
    for (size_t i = 0; i < path1.size(); i++) {
        cout << "(" << path1[i].first << "," << path1[i].second << ") ";
    }
    cout << endl;
    bool safe1 = detector.isPathSafe(path1);
    cout << "Result: " << (safe1 ? "✅ SAFE" : "❌ UNSAFE") << "\n";
    
    cout << "\nChecking Path 2: ";
    for (size_t i = 0; i < path2.size(); i++) {
        cout << "(" << path2[i].first << "," << path2[i].second << ") ";
    }
    cout << endl;
    bool safe2 = detector.isPathSafe(path2);
    cout << "Result: " << (safe2 ? "✅ SAFE" : "❌ UNSAFE") << "\n";
    
    // Test individual cell checks
    cout << "\n=== INDIVIDUAL CELL CHECKS (O(1) lookup) ===" << endl;
    
    int testCells[][2] = {{25, 15}, {40, 20}, {5, 5}, {50, 10}};
    int numCells = 4;
    
    for (int i = 0; i < numCells; i++) {
        int x = testCells[i][0];
        int y = testCells[i][1];
        bool hazard = detector.isHazard(x, y);
        cout << "Cell (" << x << "," << y << "): " 
             << (hazard ? "🚨 HAZARD" : "✅ Clear") << endl;
        
        if (hazard) {
            Hazard* h = detector.getHazard(x, y);
            if (h) {
                cout << "   Type: " << h->type 
                     << " | Time left: " << h->timeRemaining() << "s" << endl;
            }
        }
    }
    
    // Simulate time passing
    cout << "\n=== SIMULATING 20 SECONDS PASSING ===" << endl;
    sleep_seconds(20);
    
    // Clean expired hazards
    cout << "\nCleaning expired hazards..." << endl;
    detector.cleanExpiredHazards();
    detector.displayStatus();
    
    // Save to file
    cout << "\n=== SAVING TO FILE ===" << endl;
    detector.saveToFile("hazard_log.txt");
    
    // Remove a specific hazard
    cout << "\n=== MANUAL HAZARD REMOVAL ===" << endl;
    detector.removeHazard(40, 20);
    detector.displayStatus();
    
    // O(1) performance demonstration
    cout << "\n=== O(1) PERFORMANCE TEST ===" << endl;
    cout << "Checking 10,000 random cells..." << endl;
    
    int hazardCount = 0;
    for (int i = 0; i < 10000; i++) {
        int x = i % 60;
        int y = (i * 7) % 40;
        if (detector.isHazard(x, y)) {
            hazardCount++;
        }
    }
    
    cout << "Found " << hazardCount << " hazard hits in 10,000 O(1) lookups" << endl;
    
    cout << "\n✅ Hazard Detection Demo Complete!\n" << endl;
    
    return 0;
}

