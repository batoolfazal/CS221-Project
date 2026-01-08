# Drone Telemetry Logger System

## Overview

Comprehensive telemetry logging and flight data management system for drone operations. Tracks flight data, manages waypoints, logs errors, and simulates drone control systems.

## Features

### Data Structures
- **Doubly Linked List** - Flight data with bidirectional traversal
- **Singly Linked List** - Waypoint buffer for navigation points
- **Hash Map** - Fast flight data retrieval by timestamp
- **Vector** - Motor control and sensor data storage

### Core Components

#### TelemetryLogger (Master Header)
- `DroneLogger` - Main flight data logger with CSV I/O
- `WaypointBuffer` - Navigation waypoint management
- `FlightHashMap` - Fast data lookup and caching
- `ErrorLog` - File and console error logging

#### DroneControl (Master Header)
- `QuadcopterSystem` - Multi-motor quadcopter control
- `MotorControl` - PWM calculation and safety checks

#### DroneSensors (Master Header)
- `GPSData` - GPS coordinate tracking with distance calculations
- `IMUData` - Inertial measurement unit (roll, pitch, yaw)

## File Structure

```
TelemetryLogger/
├── TelemetryLogger.h      - Master header (logger components)
├── DroneControl.h         - Master header (control components)
├── DroneSensors.h         - Master header (sensor components)
├── DroneLogger.h          - Flight data logger interface
├── DroneLogger.cpp        - Flight data logger implementation
├── WaypointBuffer.h       - Waypoint buffer interface
├── WaypointBuffer.cpp     - Waypoint buffer implementation
├── FlightHashMap.h        - Hash map interface
├── FlightHashMap.cpp      - Hash map implementation
├── ErrorLog.h             - Error logging interface
├── ErrorLog.cpp           - Error logging implementation
├── Quadcopter.h           - Quadcopter system interface
├── Quadcopter.cpp         - Quadcopter system implementation
├── MotorControl.h         - Motor control utilities
├── MotorControl.cpp       - Motor control implementation
├── GPSData.h              - GPS data interface
├── GPSData.cpp            - GPS data implementation
├── IMUData.h              - IMU data interface
├── IMUData.cpp            - IMU data implementation
├── main_telemetry.cpp     - Main program entry point
├── GenerateDroneFlightData.py - Flight data generator
├── Makefile               - Build system
└── README.md              - This file
```

## Build Instructions

### Prerequisites
- C++11 or later compiler (g++ recommended)
- Python 3.x (for data generation)
- Make utility

### Building

```bash
cd TelemetryLogger
make              # Build the executable
make generate     # Generate flight data
make run          # Build, generate, and run
make clean        # Remove build artifacts
```

## Usage

1. **Generate Flight Data**:
   ```bash
   make generate
   ```
   Creates `DroneFlightLog.csv` with 50 waypoints

2. **Run the Program**:
   ```bash
   make run
   ```
   Or build and run separately:
   ```bash
   make
   ./drone_telemetry
   ```

3. **Clean Build Files**:
   ```bash
   make clean
   ```

## Data Flow

```
main_telemetry.cpp
    ↓ includes
    ├─► TelemetryLogger.h (Master)
    │       ↓ includes
    │       ├─► DroneLogger.h → DroneLogger.cpp
    │       ├─► WaypointBuffer.h → WaypointBuffer.cpp
    │       ├─► FlightHashMap.h → FlightHashMap.cpp
    │       └─► ErrorLog.h → ErrorLog.cpp
    │
    ├─► DroneControl.h (Master)
    │       ↓ includes
    │       ├─► Quadcopter.h → Quadcopter.cpp
    │       └─► MotorControl.h → MotorControl.cpp
    │
    └─► DroneSensors.h (Master)
            ↓ includes
            ├─► GPSData.h → GPSData.cpp
            └─► IMUData.h → IMUData.cpp
```

## Performance

- **Flight Data Loading**: O(N) where N = number of data points
- **Waypoint Insert**: O(1) - Insert at tail
- **Hash Map Lookup**: O(1) average case
- **Reverse Traversal**: O(N) - Using doubly linked list prev pointers
- **Distance Calculation**: O(1) - Haversine formula

## Output Files

- `DroneFlightLog.csv` - Generated flight data (timestamp, GPS, IMU, battery, etc.)
- `drone_errors.log` - Error and info log entries with timestamps
- `drone_telemetry` - Compiled executable

## Key Algorithms

- **Haversine Formula** - GPS distance calculation between coordinates
- **Doubly Linked List** - Bidirectional traversal for efficient reverse access
- **Hash Map** - Fast timestamp-based data retrieval
- **PWM Control** - Motor stabilization with roll/pitch corrections

## Requirements

- C++11 or later
- Python 3.x (for data generation)
- Make utility
- Standard C++ libraries (iostream, fstream, vector, unordered_map, etc.)

