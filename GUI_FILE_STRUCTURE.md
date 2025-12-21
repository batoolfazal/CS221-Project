# GUI Feature - File Structure & Execution Flow

## 📁 File Structure

```
CS221-Project/
│
├── include/
│   ├── gui.h                    ⭐ GUI Header (Interface)
│   ├── telemetry.h              📊 Telemetry data structures
│   ├── CampusMap.h             🗺️  Map data structures
│   └── HazardDetector.h        ⚠️  Hazard detection structures
│
├── src/
│   ├── main.cpp                🚀 MAIN ENTRY POINT (Runs First!)
│   ├── gui.cpp                 🎨 GUI Implementation
│   ├── telemetry.cpp           📊 Telemetry implementation
│   ├── CampusMap.cpp           🗺️  Map implementation
│   └── (other mission files...)
│
├── HazardDetectionUpdated/
│   ├── HazardDetector.h        ⚠️  Hazard header
│   └── HazardDetector.cpp      ⚠️  Hazard implementation
│
└── Mapping/
    └── CampusMap.h             🗺️  Map header (alternative location)
```

## 🔄 Execution Flow (Which File Runs First)

```
┌─────────────────────────────────────────────────────────────┐
│                    EXECUTION ORDER                          │
└─────────────────────────────────────────────────────────────┘

1️⃣  main.cpp (ENTRY POINT)
    │
    ├─► Loads Campus Map
    ├─► Sets up NFZ Database
    ├─► Initializes Hazard Detector
    ├─► Runs DFS/BFS/A* Pathfinding
    ├─► Handles Hazard Rerouting
    │
    ├─► Writes mission_summary.txt ✅ (BEFORE GUI)
    │
    └─► Calls displayMissionGUI() from gui.cpp
         │
         └─► gui.cpp (GUI Implementation)
              │
              ├─► Includes gui.h (header)
              ├─► Includes NFZ.h
              ├─► Includes SFML (if USE_SFML defined)
              │
              └─► Opens SFML Window (BLOCKS until closed)
```

## 📋 File Dependencies (Include Chain)

```
gui.h
  ├── #include "telemetry.h"        → TelemetryEntry, TelemetryLog
  ├── #include "CampusMap.h"         → CampusMap class
  └── #include "HazardDetector.h"    → Hazard struct, HazardDetector class

gui.cpp
  ├── #include "gui.h"               → Function declaration
  ├── #include "NFZ.h"                → NFZ database functions
  └── #include <SFML/Graphics.hpp>   → SFML library (if USE_SFML)

main.cpp
  ├── #include "gui.h"                → displayMissionGUI() declaration
  ├── #include "CampusMap.h"         → Map operations
  ├── #include "HazardDetector.h"     → Hazard operations
  ├── #include "telemetry.h"          → Path data
  └── #include "NFZ.h"                → NFZ operations
```

## 🎯 Core GUI Files

### **1. `include/gui.h`** - Header File
- **Purpose**: Function declaration and data structures
- **Contains**:
  - `Waypoint` struct (row, col)
  - `displayMissionGUI()` function signature
  - Includes for dependencies (telemetry, map, hazards)

### **2. `src/gui.cpp`** - Implementation File
- **Purpose**: Actual GUI rendering code
- **Contains**:
  - SFML window creation
  - Rendering loop (5 layers)
  - Event handling (close, ESC key)
  - Color definitions
  - Drawing logic

### **3. `src/main.cpp`** - Main Program
- **Purpose**: Mission orchestrator that calls GUI
- **Contains**:
  - Mission execution logic
  - Pathfinding (A*)
  - Hazard detection/rerouting
  - Mission summary writing
  - **GUI call** (line ~475)

## 🔗 Data Flow

```
main.cpp collects data:
  │
  ├─► CampusMap (map data)
  ├─► TelemetryEntry[] (path from A*)
  ├─► Waypoint[] (start, goal, waypoints)
  └─► Hazard[] (from HazardDetector)
       │
       └─► Passes to displayMissionGUI()
            │
            └─► gui.cpp renders in SFML window
```

## 📊 What Each File Does

| File | Role | Runs When |
|------|------|-----------|
| **main.cpp** | 🚀 Entry point, mission execution | **FIRST** - Program starts here |
| **gui.h** | 📝 Header/interface definition | Compile time |
| **gui.cpp** | 🎨 GUI rendering implementation | Called from main.cpp |
| **telemetry.h/cpp** | 📊 Path data structures | Used by both main & GUI |
| **CampusMap.h/cpp** | 🗺️ Map data | Used by both main & GUI |
| **HazardDetector.h/cpp** | ⚠️ Hazard detection | Used by both main & GUI |
| **NFZ.h** | 🚫 No-Fly Zone database | Used by both main & GUI |

## 🎬 Complete Execution Sequence

```
1. Program Starts
   └─► main.cpp::main() executes

2. Mission Setup (main.cpp)
   ├─► Load campus map
   ├─► Initialize NFZ database
   ├─► Create HazardDetector
   └─► Get start/goal/waypoints

3. Pathfinding (main.cpp)
   ├─► DFS pre-check
   ├─► BFS connectivity
   ├─► A* pathfinding
   └─► Hazard rerouting (if needed)

4. Mission Summary (main.cpp)
   └─► Write mission_summary.txt ✅

5. GUI Launch (main.cpp → gui.cpp)
   ├─► main.cpp calls displayMissionGUI()
   ├─► gui.cpp creates SFML window
   ├─► gui.cpp enters rendering loop
   └─► Blocks until window closed

6. Program Exit
   └─► Returns from main()
```

## 🔑 Key Points

1. **main.cpp runs FIRST** - It's the entry point
2. **gui.cpp is called LATER** - Only after mission completes
3. **Mission summary is written BEFORE GUI** - Ensures file is complete
4. **GUI blocks execution** - Program waits for user to close window
5. **All data flows: main.cpp → gui.cpp** - GUI receives pre-computed data

## 📦 External Dependencies

- **SFML Library**: Graphics rendering (optional, requires `-DUSE_SFML`)
- **Standard C++**: Vector, string, iostream
- **Project Headers**: telemetry.h, CampusMap.h, HazardDetector.h, NFZ.h

