/**
 * Dynamic Hazard Detection using Hash Map
 * 
 * This module implements a HazardManager class that uses unordered_map
 * for O(1) hazard lookup, addition, and removal.
 * 
 * Integrates with GridMapLoader for complete safety checking.
 * 
 * DSA Course Project - GIKI Institute
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>  // Using map instead of unordered_map for older compilers

using namespace std;

// ============ CELL TYPE CONSTANTS ============
const int CELL_FREE_PATH = 0;
const int CELL_BUILDING = 1;
const int CELL_NO_FLY_ZONE = 2;
const int CELL_HAZARD = 3;

// ============ GRID MAP LOADER CLASS ============
class GridMapLoader {
private:
    int rows;
    int cols;
    vector<vector<int> > grid;
    bool loaded;
    string filename;

public:
    GridMapLoader() : rows(0), cols(0), loaded(false), filename("") {}
    
    ~GridMapLoader() { grid.clear(); }
    
    bool loadFromFile(const char* file) {
        filename = file;
        ifstream inFile(file);
        
        if (!inFile.is_open()) {
            cout << "Error: Cannot open file '" << file << "'" << endl;
            return false;
        }
        
        if (!(inFile >> rows >> cols)) {
            cout << "Error: Failed to read grid dimensions." << endl;
            inFile.close();
            return false;
        }
        
        if (rows <= 0 || cols <= 0) {
            cout << "Error: Invalid dimensions." << endl;
            inFile.close();
            return false;
        }
        
        grid.clear();
        grid.resize(rows, vector<int>(cols, 0));
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (!(inFile >> grid[i][j])) {
                    cout << "Error: Missing data at (" << i << "," << j << ")" << endl;
                    inFile.close();
                    loaded = false;
                    return false;
                }
            }
        }
        
        inFile.close();
        loaded = true;
        cout << "Loaded grid: " << rows << "x" << cols << endl;
        return true;
    }
    
    bool isLoaded() const { return loaded; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    
    bool isInBounds(int r, int c) const {
        return (r >= 0 && r < rows && c >= 0 && c < cols);
    }
    
    int getCellValue(int r, int c) const {
        if (!isInBounds(r, c)) return -1;
        return grid[r][c];
    }
    
    bool isFree(int r, int c) const {
        return isInBounds(r, c) && grid[r][c] == CELL_FREE_PATH;
    }
    
    bool isObstacle(int r, int c) const {
        return isInBounds(r, c) && grid[r][c] == CELL_BUILDING;
    }
    
    bool isNoFlyZone(int r, int c) const {
        return isInBounds(r, c) && grid[r][c] == CELL_NO_FLY_ZONE;
    }
    
    bool isStaticHazard(int r, int c) const {
        return isInBounds(r, c) && grid[r][c] == CELL_HAZARD;
    }
};

// ============ HAZARD MANAGER CLASS ============
class HazardManager {
private:
    // Hash Map: Key = "row,col", Value = hazard description
    map<string, string> hazardMap;
    
    // Helper: Generate key from coordinates - O(1) string creation
    string generateKey(int r, int c) const {
        // Convert coordinates to string key format "r,c"
        string key = "";
        
        // Handle negative numbers
        if (r < 0) {
            key += "-";
            r = -r;
        }
        
        // Convert row to string
        if (r == 0) {
            key += "0";
        } else {
            string temp = "";
            while (r > 0) {
                temp = char('0' + r % 10) + temp;
                r /= 10;
            }
            key += temp;
        }
        
        key += ",";
        
        // Handle negative column
        if (c < 0) {
            key += "-";
            c = -c;
        }
        
        // Convert col to string
        if (c == 0) {
            key += "0";
        } else {
            string temp = "";
            while (c > 0) {
                temp = char('0' + c % 10) + temp;
                c /= 10;
            }
            key += temp;
        }
        
        return key;
    }

public:
    // Constructor
    HazardManager() {}
    
    // Destructor
    ~HazardManager() {
        hazardMap.clear();
    }
    
    // Add a dynamic hazard at (r, c) with description
    void addHazard(int r, int c, const string& description) {
        string key = generateKey(r, c);
        hazardMap[key] = description;
        cout << "[HAZARD ADDED] Location (" << r << "," << c << "): " << description << endl;
    }
    
    // Remove hazard at (r, c)
    bool removeHazard(int r, int c) {
        string key = generateKey(r, c);
        
        if (hazardMap.find(key) != hazardMap.end()) {
            string desc = hazardMap[key];
            hazardMap.erase(key);
            cout << "[HAZARD REMOVED] Location (" << r << "," << c << "): " << desc << endl;
            return true;
        }
        
        cout << "[WARNING] No hazard found at (" << r << "," << c << ")" << endl;
        return false;
    }
    
    // Check if coordinate has a dynamic hazard - O(1) lookup
    bool isHazard(int r, int c) const {
        string key = generateKey(r, c);
        return hazardMap.find(key) != hazardMap.end();
    }
    
    // Get hazard description at (r, c)
    string getHazardDescription(int r, int c) const {
        string key = generateKey(r, c);
        
        if (hazardMap.find(key) != hazardMap.end()) {
            return hazardMap.find(key)->second;
        }
        return "";
    }
    
    // Check if location is safe (combines static grid + dynamic hazards)
    bool isLocationSafe(int r, int c, const GridMapLoader& loader) const {
        // Check bounds first
        if (!loader.isInBounds(r, c)) {
            cout << "  -> OUT OF BOUNDS" << endl;
            return false;
        }
        
        // Check static obstacles (buildings)
        if (loader.isObstacle(r, c)) {
            cout << "  -> BLOCKED: Building/Wall" << endl;
            return false;
        }
        
        // Check static no-fly zones
        if (loader.isNoFlyZone(r, c)) {
            cout << "  -> BLOCKED: No-Fly Zone" << endl;
            return false;
        }
        
        // Check static hazards from grid
        if (loader.isStaticHazard(r, c)) {
            cout << "  -> BLOCKED: Static Hazard" << endl;
            return false;
        }
        
        // Check dynamic hazards from hash map - O(1) lookup
        if (isHazard(r, c)) {
            cout << "  -> BLOCKED: Dynamic Hazard (" << getHazardDescription(r, c) << ")" << endl;
            return false;
        }
        
        return true;
    }
    
    // Get total number of active dynamic hazards
    int getHazardCount() const {
        return hazardMap.size();
    }
    
    // Print all active hazards
    void printAllHazards() const {
        cout << "\n========== ACTIVE DYNAMIC HAZARDS ==========" << endl;
        
        if (hazardMap.empty()) {
            cout << "No active hazards." << endl;
        } else {
            cout << "Total: " << hazardMap.size() << " hazard(s)" << endl;
            cout << "--------------------------------------------" << endl;
            
            int count = 1;
            for (map<string, string>::const_iterator it = hazardMap.begin(); 
                 it != hazardMap.end(); ++it) {
                cout << count++ << ". Location [" << it->first << "]: " << it->second << endl;
            }
        }
        cout << "=============================================" << endl;
    }
    
    // Clear all hazards
    void clearAllHazards() {
        int count = hazardMap.size();
        hazardMap.clear();
        cout << "[CLEARED] Removed " << count << " hazard(s)" << endl;
    }
};

// ============ MAIN FUNCTION ============
int main() {
    ios_base::sync_with_stdio(false);
    
    cout << "=== GIKI Campus Hazard Detection System ===" << endl;
    cout << "Using Hash Map for O(1) Hazard Lookup" << endl;
    cout << endl;
    
    // Load the static grid map
    GridMapLoader loader;
    if (!loader.loadFromFile("giki_map.txt")) {
        cout << "Failed to load map. Run Grid.cpp first." << endl;
        return 1;
    }
    
    // Create hazard manager
    HazardManager hazardMgr;
    
    cout << "\n=== Adding Dynamic Hazards ===" << endl;
    
    // Simulate dynamic hazards
    hazardMgr.addHazard(40, 45, "Steam Leak");
    hazardMgr.addHazard(50, 30, "Gas Leak");
    hazardMgr.addHazard(35, 40, "Fallen Tree");
    hazardMgr.addHazard(60, 52, "Water Flooding");
    hazardMgr.addHazard(25, 33, "Electrical Fault");
    
    // Print all hazards
    hazardMgr.printAllHazards();
    
    // Test safety checks
    cout << "\n=== Safety Check Tests ===" << endl;
    
    // Test locations
    int testLocations[][2] = {
        {40, 45},   // Steam Leak - dynamic hazard
        {50, 30},   // Gas Leak - dynamic hazard
        {5, 5},     // Should be free path
        {0, 0},     // Boundary wall - static obstacle
        {10, 35},   // No-fly zone (Admin)
        {74, 40},   // Near main gate - should be free
        {63, 5},    // Static hazard (construction zone)
        {25, 33}    // Electrical fault - dynamic hazard
    };
    
    for (int i = 0; i < 8; i++) {
        int r = testLocations[i][0];
        int c = testLocations[i][1];
        
        cout << "\nChecking (" << r << "," << c << "):" << endl;
        bool safe = hazardMgr.isLocationSafe(r, c, loader);
        cout << "  Result: " << (safe ? "SAFE" : "UNSAFE") << endl;
    }
    
    // Test hazard removal
    cout << "\n=== Hazard Removal Test ===" << endl;
    hazardMgr.removeHazard(40, 45);  // Remove steam leak
    
    // Re-check the location
    cout << "\nRe-checking (40,45) after removal:" << endl;
    bool safeNow = hazardMgr.isLocationSafe(40, 45, loader);
    cout << "  Result: " << (safeNow ? "SAFE" : "UNSAFE") << endl;
    
    // Print remaining hazards
    hazardMgr.printAllHazards();
    
    // Test O(1) lookup performance demonstration
    cout << "\n=== O(1) Lookup Demonstration ===" << endl;
    cout << "Checking 1000 random locations..." << endl;
    
    int hazardHits = 0;
    for (int i = 0; i < 1000; i++) {
        int r = i % 80;
        int c = (i * 7) % 80;
        if (hazardMgr.isHazard(r, c)) {
            hazardHits++;
        }
    }
    cout << "Found " << hazardHits << " hazard hits in 1000 lookups (O(1) each)" << endl;
    
    cout << "\n=== Hazard Detection Complete ===" << endl;
    
    return 0;
}

