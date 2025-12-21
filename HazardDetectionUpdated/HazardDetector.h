#ifndef HAZARD_DETECTOR_H
#define HAZARD_DETECTOR_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <utility>

using namespace std;

// ============================================
// HAZARD STRUCTURE
// ============================================
struct Hazard {
    int x, y;
    string type;         // "BIRD", "DRONE", "WEATHER", "DEBRIS"
    string description;  // Detailed description
    time_t timestamp;    // When detected
    int duration;        // How long it lasts (seconds)
    int severity;        // 1-5 (5 = most dangerous)
    
    Hazard();
    Hazard(int xPos, int yPos, string hazardType, string desc, int dur, int sev);
    
    bool isActive() const;
    int timeRemaining() const;
    void display() const;
};

// ============================================
// HAZARD DETECTOR CLASS
// ============================================
class HazardDetector {
private:
    unordered_map<int, Hazard> hazardMap;
    int mapWidth;
    int mapHeight;
    int totalDetected;
    int totalExpired;
    
    // Hash function: (x,y) → unique int
    int hashKey(int x, int y) const;
    
    // Reverse: hash key → (x,y)
    pair<int, int> reverseHash(int key) const;
    
public:
    HazardDetector(int width, int height);
    ~HazardDetector();
    
    // Core operations (O(1))
    bool addHazard(int x, int y, string type, string description, int duration, int severity);
    bool isHazard(int x, int y) const;
    Hazard* getHazard(int x, int y);
    bool removeHazard(int x, int y);
    
    // Maintenance
    int cleanExpiredHazards();
    void clearAllHazards();
    
    // Path safety
    bool isPathSafe(const vector<pair<int, int> >& path) const;
    
    // Queries
    vector<Hazard> getActiveHazards() const;
    int getActiveCount() const;
    int getTotalDetected() const;
    int getTotalExpired() const;
    
    // I/O
    void displayStatus() const;
    void displayAllHazards() const;
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);

    // Generate random hazards using predefined type profiles (similar to GenerateHazards.py)
    void generateRandomHazards(int count);
};

// Global pointer for integration with A* validation
extern HazardDetector* gHazardDetector;

// C-style wrappers for easy calls
bool hdAddHazard(int x, int y, const char* type, const char* desc, int duration, int severity);
bool hdRemoveHazard(int x, int y);
bool hdIsHazard(int x, int y);
void hdGenerateRandomHazards(int count);

#endif // HAZARD_DETECTOR_H

