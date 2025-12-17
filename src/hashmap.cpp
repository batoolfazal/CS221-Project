#ifndef HAZARD_DETECTOR_HPP
#define HAZARD_DETECTOR_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

// ============================================
// HAZARD STRUCTURE
// ============================================
struct Hazard {
    int x, y;
    string type;        // "BIRD", "DRONE", "WEATHER", "DEBRIS"
    time_t timestamp;   // When detected
    int duration;       // How long it lasts (seconds)
    int severity;       // 1-5 (5 = most dangerous)
    
    Hazard() : x(0), y(0), type("UNKNOWN"), timestamp(0), duration(0), severity(1) {}
    
    Hazard(int xPos, int yPos, string hazardType, int dur, int sev) 
        : x(xPos), y(yPos), type(hazardType), duration(dur), severity(sev) {
        timestamp = time(nullptr);
    }
    
    // Check if hazard is still active
    bool isActive() const {
        time_t now = time(nullptr);
        return (now - timestamp) < duration;
    }
    
    // Time remaining
    int timeRemaining() const {
        time_t now = time(nullptr);
        int elapsed = now - timestamp;
        return max(0, duration - elapsed);
    }
};

// ============================================
// HASH MAP HAZARD DETECTOR
// ============================================
class HazardDetector {
private:
    unordered_map<int, Hazard> hazardMap;
    int mapWidth;
    int mapHeight;
    int totalDetected;
    int totalExpired;
    
    // Hash function: convert (x,y) to unique integer key
    int hashKey(int x, int y) const {
        return y * mapWidth + x;
    }
    
    // Reverse: convert hash key back to (x,y)
    pair<int, int> reverseHash(int key) const {
        int y = key / mapWidth;
        int x = key % mapWidth;
        return {x, y};
    }
    
public:
    HazardDetector(int width, int height) 
        : mapWidth(width), mapHeight(height), totalDetected(0), totalExpired(0) {
        cout << "✅ HazardDetector initialized (" << width << "x" << height << ")" << endl;
    }
    
    // ============================================
    // ADD NEW HAZARD
    // ============================================
    bool addHazard(int x, int y, string type, int duration, int severity) {
        // Validate coordinates
        if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
            cerr << "❌ Invalid coordinates: (" << x << "," << y << ")" << endl;
            return false;
        }
        
        int key = hashKey(x, y);
        
        // Check if hazard already exists at this location
        if (hazardMap.find(key) != hazardMap.end()) {
            cout << "⚠️  Hazard already exists at (" << x << "," << y << ")" << endl;
            cout << "   Updating with new data..." << endl;
        }
        
        // Insert hazard (O(1) operation!)
        hazardMap[key] = Hazard(x, y, type, duration, severity);
        totalDetected++;
        
        cout << "🚨 HAZARD DETECTED at (" << x << "," << y << ")" << endl;
        cout << "   Type: " << type << endl;
        cout << "   Severity: " << severity << "/5" << endl;
        cout << "   Duration: " << duration << "s" << endl;
        
        return true;
    }
    
    // ============================================
    // CHECK IF CELL HAS HAZARD (O(1))
    // ============================================
    bool isHazard(int x, int y) const {
        int key = hashKey(x, y);
        
        // Hash map lookup: O(1)
        auto it = hazardMap.find(key);
        
        if (it == hazardMap.end()) {
            return false;  // No hazard here
        }
        
        // Found hazard, but is it still active?
        return it->second.isActive();
    }
    
    // ============================================
    // GET HAZARD INFO
    // ============================================
    Hazard* getHazard(int x, int y) {
        int key = hashKey(x, y);
        auto it = hazardMap.find(key);
        
        if (it != hazardMap.end() && it->second.isActive()) {
            return &(it->second);
        }
        
        return nullptr;
    }
    
    // ============================================
    // REMOVE HAZARD (manual removal)
    // ============================================
    bool removeHazard(int x, int y) {
        int key = hashKey(x, y);
        
        if (hazardMap.find(key) != hazardMap.end()) {
            hazardMap.erase(key);  // O(1) deletion
            cout << "✅ Hazard cleared at (" << x << "," << y << ")" << endl;
            return true;
        }
        
        cout << "⚠️  No hazard at (" << x << "," << y << ")" << endl;
        return false;
    }
    
    // ============================================
    // CLEAN EXPIRED HAZARDS
    // ============================================
    int cleanExpiredHazards() {
        int removed = 0;
        
        // Iterator-safe removal
        for (auto it = hazardMap.begin(); it != hazardMap.end(); ) {
            if (!it->second.isActive()) {
                auto pos = reverseHash(it->first);
                cout << "🧹 Expired hazard removed at (" << pos.first << "," << pos.second << ")" << endl;
                it = hazardMap.erase(it);  // O(1)
                removed++;
                totalExpired++;
            } else {
                ++it;
            }
        }
        
        if (removed > 0) {
            cout << "✅ Cleaned " << removed << " expired hazards" << endl;
        }
        
        return removed;
    }
    
    // ============================================
    // CHECK IF PATH IS SAFE
    // ============================================
    bool isPathSafe(const vector<pair<int, int>>& path) const {
        for (const auto& [x, y] : path) {
            if (isHazard(x, y)) {
                cout << "⚠️  Path crosses hazard at (" << x << "," << y << ")" << endl;
                return false;
            }
        }
        return true;
    }
    
    // ============================================
    // GET ALL ACTIVE HAZARDS
    // ============================================
    vector<Hazard> getActiveHazards() const {
        vector<Hazard> active;
        
        for (const auto& [key, hazard] : hazardMap) {
            if (hazard.isActive()) {
                active.push_back(hazard);
            }
        }
        
        return active;
    }
    
    // ============================================
    // DISPLAY STATUS
    // ============================================
    void displayStatus() const {
        cout << "\n=== HAZARD DETECTION SYSTEM STATUS ===" << endl;
        cout << "Active hazards: " << hazardMap.size() << endl;
        cout << "Total detected: " << totalDetected << endl;
        cout << "Total expired: " << totalExpired << endl;
        
        if (hazardMap.empty()) {
            cout << "✅ No active hazards - Airspace clear!" << endl;
        } else {
            cout << "\n🚨 ACTIVE HAZARDS:" << endl;
            for (const auto& [key, hazard] : hazardMap) {
                if (hazard.isActive()) {
                    cout << "  (" << hazard.x << "," << hazard.y << ") - "
                         << hazard.type << " [Severity: " << hazard.severity 
                         << "/5, Time left: " << hazard.timeRemaining() << "s]" << endl;
                }
            }
        }
        cout << "======================================\n" << endl;
    }
    
    // ============================================
    // GET STATISTICS
    // ============================================
    int getActiveCount() const { 
        return hazardMap.size(); 
    }
    
    int getTotalDetected() const { 
        return totalDetected; 
    }
    
    int getTotalExpired() const { 
        return totalExpired; 
    }
    
    // ============================================
    // SAVE HAZARDS TO FILE (for logging)
    // ============================================
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "❌ Cannot save hazards to file" << endl;
            return;
        }
        
        file << "# Hazard Detection Log\n";
        file << "# Format: X Y TYPE DURATION SEVERITY TIMESTAMP\n\n";
        
        for (const auto& [key, hazard] : hazardMap) {
            if (hazard.isActive()) {
                file << hazard.x << " " << hazard.y << " "
                     << hazard.type << " " << hazard.duration << " "
                     << hazard.severity << " " << hazard.timestamp << "\n";
            }
        }
        
        file.close();
        cout << "💾 Hazards saved to '" << filename << "'" << endl;
    }
};

#endif // HAZARD_DETECTOR_HPP
