#ifndef GUI_H
#define GUI_H

// Optional SFML visualization for the mission system.
// Build with -DUSE_SFML and link against SFML if available.

#include <vector>
#include <string>
#include "telemetry.h"
#include "CampusMap.h"
#include "HazardDetector.h"

struct Waypoint {
    int row;
    int col;
};

// Launches an SFML window to display the map, path, waypoints, hazards.
// If SFML is not available or USE_SFML is not defined, this becomes a no-op.
void displayMissionGUI(
    CampusMap& map,
    const std::vector<TelemetryEntry>& path,
    const Waypoint& start,
    const Waypoint& goal,
    const std::vector<Waypoint>& waypoints,
    const std::vector<Hazard>& hazards);

#endif // GUI_H

