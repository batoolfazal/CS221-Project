Dynamic Hazard Detection System
Overview

Tracks hazards in a 2D airspace grid and checks for safety in real-time.
Handles multiple hazard types, severity levels, and automatically removes expired hazards.

Features

O(1) Hazard Lookup – Instantly check any position

Time-Based Expiration – Hazards disappear after their duration

Multiple Types – BIRD, DRONE, WEATHER, DEBRIS, AIRCRAFT

Severity Levels – 1-5 danger rating

Path Safety – Check if a full path is safe

File I/O – Save and load hazard data

How it Works

Uses a grid-based lookup for fast access

Hazards can be added, checked, expired, or removed manually

Includes a Python generator to create test hazard data

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


## Performance
- **Lookup**: O(1) average
- **Insert**: O(1) average
- **Delete**: O(1) average
- **Clean Expired**: O(N) where N = total hazards

## Requirements
- C++11 or later
- Python 3.x (for generator)
- Make utility

