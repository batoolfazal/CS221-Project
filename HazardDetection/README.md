# D-2: Dynamic Hazard Detection System

## Overview
Real-time hazard detection using hash maps for O(1) lookup performance.

## Features
- ✅ **O(1) Hazard Lookup** - Instant position checking
- ✅ **Time-Based Expiration** - Hazards auto-expire
- ✅ **Multiple Types** - BIRD, DRONE, WEATHER, DEBRIS, AIRCRAFT
- ✅ **Severity Levels** - 1-5 danger rating
- ✅ **Path Safety** - Check entire paths at once
- ✅ **File I/O** - Save/load hazard data

## DSA Concept
**Hash Map (unordered_map)** with integer keys for O(1) average-time lookup.

## File Structure
```
HazardDetection/
├── HazardDetector.h       - Interface
├── HazardDetector.cpp     - Implementation
├── demo_hazards.cpp       - Demo program
├── GenerateHazards.py     - Data generator
├── HazardMakefile         - Build system
└── README.md              - This file
```

## Compilation

### Quick Start
```bash
cd HazardDetection
make -f HazardMakefile run
```

### Manual Steps
```bash
# Generate data
python3 GenerateHazards.py

# Build
make -f HazardMakefile

# Run
./hazard_demo
```

## Usage Example
```cpp
#include "HazardDetector.h"

// Initialize for 60x40 map
HazardDetector detector(60, 40);

// Add hazard
detector.addHazard(25, 15, "BIRD", "Flock crossing", 15, 3);

// Check position (O(1))
if (detector.isHazard(25, 15)) {
    cout << "Hazard detected!" << endl;
}

// Check path safety
vector<pair<int, int> > path;
path.push_back(make_pair(10, 10));
path.push_back(make_pair(20, 20));
path.push_back(make_pair(30, 30));
bool safe = detector.isPathSafe(path);

// Clean expired
detector.cleanExpiredHazards();
```

## Integration with Main Project

Add to your pathfinding:
```cpp
#include "../HazardDetection/HazardDetector.h"

HazardDetector hazards(mapWidth, mapHeight);

// During A* pathfinding
if (hazards.isHazard(x, y)) {
    // Skip this cell
    continue;
}
```

## Performance
- **Lookup**: O(1) average
- **Insert**: O(1) average
- **Delete**: O(1) average
- **Clean Expired**: O(N) where N = total hazards

## Requirements
- C++11 or later
- Python 3.x (for generator)
- Make utility

