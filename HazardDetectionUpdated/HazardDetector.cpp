#include "HazardDetector.h"
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

extern HazardDetector* gHazardDetector;
// ============================================
// HAZARD IMPLEMENTATION
// ============================================

Hazard::Hazard() 
    : x(0), y(0), type("UNKNOWN"), description(""), 
      timestamp(0), duration(0), severity(1) {}

Hazard::Hazard(int xPos, int yPos, string hazardType, string desc, int dur, int sev)
    : x(xPos), y(yPos), type(hazardType), description(desc), 
      duration(dur), severity(sev) {
    timestamp = time(nullptr);
}

bool Hazard::isActive() const {
    time_t now = time(nullptr);
    return (now - timestamp) < duration;
}

int Hazard::timeRemaining() const {
    time_t now = time(nullptr);
    int elapsed = now - timestamp;
    return max(0, duration - elapsed);
}

void Hazard::display() const {
    cout << "  Location: (" << x << "," << y << ")\n";
    cout << "  Type: " << type << "\n";
    cout << "  Description: " << description << "\n";
    cout << "  Severity: " << severity << "/5\n";
    cout << "  Time remaining: " << timeRemaining() << "s\n";
}

// ============================================
// HAZARD DETECTOR IMPLEMENTATION
// ============================================

HazardDetector::HazardDetector(int width, int height)
    : mapWidth(width), mapHeight(height), totalDetected(0), totalExpired(0) {
    cout << "✅ HazardDetector initialized (" << width << "x" << height << ")" << endl;
}

HazardDetector::~HazardDetector() {
    hazardMap.clear();
}

int HazardDetector::hashKey(int x, int y) const {
    return y * mapWidth + x;
}

pair<int, int> HazardDetector::reverseHash(int key) const {
    int y = key / mapWidth;
    int x = key % mapWidth;
    return make_pair(x, y);
}

bool HazardDetector::addHazard(int x, int y, string type, string description, 
                                int duration, int severity) {
    // Validate coordinates
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        cerr << "❌ Invalid coordinates: (" << x << "," << y << ")" << endl;
        return false;
    }
    
    int key = hashKey(x, y);
    
    // Check if exists
    if (hazardMap.find(key) != hazardMap.end()) {
        cout << "⚠️  Updating existing hazard at (" << x << "," << y << ")" << endl;
    }
    
    // Insert (O(1))
    hazardMap[key] = Hazard(x, y, type, description, duration, severity);
    totalDetected++;
    
    cout << "🚨 HAZARD ADDED at (" << x << "," << y << ")" << endl;
    cout << "   Type: " << type << " | Severity: " << severity << "/5" << endl;
    cout << "   " << description << endl;
    
    return true;
}

bool HazardDetector::isHazard(int x, int y) const {
    int key = hashKey(x, y);
    auto it = hazardMap.find(key);
    
    if (it == hazardMap.end()) {
        return false;
    }
    
    return it->second.isActive();
}

Hazard* HazardDetector::getHazard(int x, int y) {
    int key = hashKey(x, y);
    auto it = hazardMap.find(key);
    
    if (it != hazardMap.end() && it->second.isActive()) {
        return &(it->second);
    }
    
    return nullptr;
}

bool HazardDetector::removeHazard(int x, int y) {
    int key = hashKey(x, y);
    
    if (hazardMap.find(key) != hazardMap.end()) {
        string desc = hazardMap[key].description;
        hazardMap.erase(key);
        cout << "✅ Hazard removed at (" << x << "," << y << "): " << desc << endl;
        return true;
    }
    
    cout << "⚠️  No hazard at (" << x << "," << y << ")" << endl;
    return false;
}

int HazardDetector::cleanExpiredHazards() {
    int removed = 0;
    
    for (auto it = hazardMap.begin(); it != hazardMap.end(); ) {
        if (!it->second.isActive()) {
            pair<int, int> pos = reverseHash(it->first);
            cout << "🧹 Expired: (" << pos.first << "," << pos.second 
                 << ") - " << it->second.type << endl;
            it = hazardMap.erase(it);
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

void HazardDetector::clearAllHazards() {
    int count = hazardMap.size();
    hazardMap.clear();
    cout << "🧹 Cleared " << count << " hazard(s)" << endl;
}

void HazardDetector::generateRandomHazards(int count) {
    if (count <= 0) return;
    struct TypeInfo {
        const char* type;
        int durMin, durMax;
        int sevMin, sevMax;
        const char* desc;
    };
    TypeInfo types[] = {
        {"BIRD", 5, 20, 2, 4, "Bird flock"},
        {"DRONE", 10, 30, 3, 5, "Unauthorized drone"},
        {"WEATHER", 30, 60, 1, 3, "Weather system"},
        {"DEBRIS", 15, 45, 3, 5, "Falling debris"},
        {"AIRCRAFT", 20, 40, 5, 5, "Aircraft nearby"}
    };
    const int typeCount = sizeof(types)/sizeof(types[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < count; i++) {
        int x = std::rand() % mapWidth;
        int y = std::rand() % mapHeight;
        int t = std::rand() % typeCount;
        TypeInfo& info = types[t];
        int duration = info.durMin + (std::rand() % (info.durMax - info.durMin + 1));
        int severity = info.sevMin + (std::rand() % (info.sevMax - info.sevMin + 1));
        std::string desc = std::string(info.desc) + " at sector " + std::to_string(i+1);
        addHazard(x, y, info.type, desc, duration, severity);
    }
}

bool HazardDetector::isPathSafe(const vector<pair<int, int> >& path) const {
    for (size_t i = 0; i < path.size(); i++) {
        int x = path[i].first;
        int y = path[i].second;
        if (isHazard(x, y)) {
            cout << "⚠️  Path crosses hazard at (" << x << "," << y << ")" << endl;
            return false;
        }
    }
    return true;
}

vector<Hazard> HazardDetector::getActiveHazards() const {
    vector<Hazard> active;
    
    for (auto it = hazardMap.begin(); it != hazardMap.end(); ++it) {
        if (it->second.isActive()) {
            active.push_back(it->second);
        }
    }
    
    return active;
}

int HazardDetector::getActiveCount() const {
    return static_cast<int>(hazardMap.size());
}

int HazardDetector::getTotalDetected() const {
    return totalDetected;
}

int HazardDetector::getTotalExpired() const {
    return totalExpired;
}

void HazardDetector::displayStatus() const {
    cout << "\n========================================" << endl;
    cout << "    HAZARD DETECTION SYSTEM STATUS" << endl;
    cout << "========================================" << endl;
    cout << "Active hazards: " << hazardMap.size() << endl;
    cout << "Total detected: " << totalDetected << endl;
    cout << "Total expired: " << totalExpired << endl;
    
    if (hazardMap.empty()) {
        cout << "✅ Airspace clear - No active hazards" << endl;
    }
    
    cout << "========================================\n" << endl;
}

void HazardDetector::displayAllHazards() const {
    cout << "\n========== ACTIVE HAZARDS ==========" << endl;
    
    if (hazardMap.empty()) {
        cout << "No active hazards." << endl;
    } else {
        int count = 1;
        for (auto it = hazardMap.begin(); it != hazardMap.end(); ++it) {
            if (it->second.isActive()) {
                cout << "\nHazard #" << count++ << ":" << endl;
                it->second.display();
            }
        }
    }
    cout << "====================================\n" << endl;
}

bool HazardDetector::saveToFile(const string& filename) const {
    ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        cerr << "❌ Cannot save to file: " << filename << endl;
        return false;
    }
    
    file << "# Hazard Detection Log\n";
    file << "# Format: X Y TYPE DESCRIPTION DURATION SEVERITY TIMESTAMP\n\n";
    
    for (auto it = hazardMap.begin(); it != hazardMap.end(); ++it) {
        if (it->second.isActive()) {
            file << it->second.x << " " << it->second.y << " "
                 << it->second.type << " \"" << it->second.description << "\" "
                 << it->second.duration << " " << it->second.severity << " "
                 << it->second.timestamp << "\n";
        }
    }
    
    file.close();
    cout << "💾 Hazards saved to '" << filename << "'" << endl;
    return true;
}

bool HazardDetector::loadFromFile(const string& filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cerr << "❌ Cannot load file: " << filename << endl;
        return false;
    }
    
    string line;
    int loaded = 0;
    
    while (getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;
        
        istringstream iss(line);
        int x, y, duration, severity;
        time_t timestamp;
        string type, description;
        
        // Read x, y, type
        if (!(iss >> x >> y >> type)) {
            continue;
        }
        
        // Read description (between quotes)
        string rest;
        getline(iss, rest);
        
        // Find first quote
        size_t firstQuote = rest.find('"');
        if (firstQuote == string::npos) {
            continue;
        }
        
        // Find second quote
        size_t secondQuote = rest.find('"', firstQuote + 1);
        if (secondQuote == string::npos) {
            continue;
        }
        
        // Extract description
        description = rest.substr(firstQuote + 1, secondQuote - firstQuote - 1);
        
        // Read duration, severity, timestamp from after second quote
        istringstream restStream(rest.substr(secondQuote + 1));
        if (!(restStream >> duration >> severity >> timestamp)) {
            continue;
        }
        
        int key = hashKey(x, y);
        hazardMap[key] = Hazard(x, y, type, description, duration, severity);
        hazardMap[key].timestamp = timestamp;
        
        loaded++;
    }
    
    file.close();
    cout << "✅ Loaded " << loaded << " hazards from '" << filename << "'" << endl;
    return true;
}

// ============ WRAPPER HELPERS ============
bool hdAddHazard(int x, int y, const char* type, const char* desc, int duration, int severity) {
    if (gHazardDetector == nullptr) return false;
    return gHazardDetector->addHazard(x, y, string(type), string(desc), duration, severity);
}

bool hdRemoveHazard(int x, int y) {
    if (gHazardDetector == nullptr) return false;
    return gHazardDetector->removeHazard(x, y);
}

bool hdIsHazard(int x, int y) {
    if (gHazardDetector == nullptr) return false;
    return gHazardDetector->isHazard(x, y);
}

void hdGenerateRandomHazards(int count) {
    if (gHazardDetector != nullptr) {
        gHazardDetector->generateRandomHazards(count);
    }
}

