#include "gui.h"
#include "NFZ.h"
#include <iostream>
#include <cmath>

// Enable SFML if available - can be overridden by compiler flag -DUSE_SFML
#ifndef USE_SFML
// Uncomment the line below if SFML is installed and you want to enable GUI
// #define USE_SFML
#endif

#ifdef USE_SFML
#include <SFML/Graphics.hpp>
#endif

void displayMissionGUI(
    CampusMap& map,
    const std::vector<TelemetryEntry>& path,
    const Waypoint& start,
    const Waypoint& goal,
    const std::vector<Waypoint>& waypoints,
    const std::vector<Hazard>& hazards
) {
#ifdef USE_SFML
    std::cout << "[GUI] Opening SFML visualizer window..." << std::endl;
    
    // Calculate window size with better scaling
    const int CELL_SIZE = 8; // Increased for better visibility (80x80 -> 640x640)
    const int WINDOW_WIDTH = map.getCols() * CELL_SIZE;
    const int WINDOW_HEIGHT = map.getRows() * CELL_SIZE;
    
    // Create window with title
    sf::RenderWindow window(
        sf::VideoMode(static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT)),
        "Mission Orchestrator - SFML Visualizer",
        sf::Style::Titlebar | sf::Style::Close
    );
    
    // Set frame rate limit for efficiency
    window.setFramerateLimit(60);

    // Color scheme for visualization
    sf::Color colorWall(30, 30, 30);              // Dark Grey - Buildings/Obstacles
    sf::Color colorFree(240, 240, 240);            // Light Grey - Free paths
    sf::Color colorNFZ(255, 100, 100, 180);        // Light Red (transparent) - No-Fly Zones
    sf::Color colorStaticHazard(200, 100, 0, 200); // Orange/Brown - Static hazards from map
    sf::Color colorDynamicHazard(255, 0, 255, 220); // Magenta - Dynamic hazards from HazardDetector
    sf::Color colorPath(0, 150, 255, 200);          // Blue (transparent) - Planned path
    sf::Color colorStart(0, 255, 0);                // Green - Start position
    sf::Color colorGoal(255, 165, 0);               // Orange - Goal position
    sf::Color colorWaypoint(255, 255, 0);          // Yellow - Waypoints
    sf::Color colorBackground(255, 255, 255);       // White background

    // Cell shape for drawing
    sf::RectangleShape cell(sf::Vector2f(static_cast<float>(CELL_SIZE - 1), static_cast<float>(CELL_SIZE - 1)));

    std::cout << "[GUI] Window opened. Close window to continue..." << std::endl;
    std::cout << "[GUI] Legend: Green=Start, Orange=Goal, Yellow=Waypoints, Blue=Path, Magenta=Dynamic Hazards, Red=NFZ" << std::endl;

    // Main rendering loop - blocks until window is closed
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                std::cout << "[GUI] Window closed by user." << std::endl;
            }
            // Allow ESC key to close window
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
                std::cout << "[GUI] Window closed (ESC key pressed)." << std::endl;
            }
        }

        // Clear window with white background
        window.clear(colorBackground);

        // ========== LAYER 1: Base Map (Buildings, Free Space, NFZ, Static Hazards) ==========
        for (int r = 0; r < map.getRows(); r++) {
            for (int c = 0; c < map.getCols(); c++) {
                cell.setPosition(sf::Vector2f(static_cast<float>(c * CELL_SIZE), static_cast<float>(r * CELL_SIZE)));
                
                // Determine cell type and color
                if (map.isObstacle(r, c)) {
                    // Building/Obstacle
                    cell.setFillColor(colorWall);
                } else if (map.isNoFlyZone(r, c) || isNFZ(r, c)) {
                    // No-Fly Zone (from map or NFZ database)
                    cell.setFillColor(colorNFZ);
                } else if (map.isHazard(r, c)) {
                    // Static hazard from map
                    cell.setFillColor(colorStaticHazard);
                } else if (map.isFree(r, c)) {
                    // Free path
                    cell.setFillColor(colorFree);
                } else {
                    // Unknown/other
                    cell.setFillColor(colorWall);
                }
                window.draw(cell);
            }
        }

        // ========== LAYER 2: Dynamic Hazards (from HazardDetector) ==========
        // Draw dynamic hazards on top of base map
        // Note: HazardDetector uses (x,y) where x=col, y=row based on initialization
        // HazardDetector is initialized with (cols, rows), so x maps to col, y maps to row
        for (const auto& h : hazards) {
            if (h.isActive()) {
                // Hazard coordinates: h.x = column, h.y = row
                int hazardRow = h.y;
                int hazardCol = h.x;
                
                if (map.isInBounds(hazardRow, hazardCol)) {
                    cell.setPosition(sf::Vector2f(
                        static_cast<float>(hazardCol * CELL_SIZE),
                        static_cast<float>(hazardRow * CELL_SIZE)
                    ));
                    cell.setFillColor(colorDynamicHazard);
                    window.draw(cell);
                }
            }
        }

        // ========== LAYER 3: Planned Path ==========
        // Draw path on top of map and hazards so it's visible
        for (const auto& step : path) {
            if (map.isInBounds(step.row, step.col)) {
                cell.setPosition(sf::Vector2f(
                    static_cast<float>(step.col * CELL_SIZE),
                    static_cast<float>(step.row * CELL_SIZE)
                ));
                cell.setFillColor(colorPath);
                window.draw(cell);
            }
        }

        // ========== LAYER 4: Waypoints ==========
        // Draw waypoints on top of path
        for (const auto& wp : waypoints) {
            if (map.isInBounds(wp.row, wp.col)) {
                cell.setPosition(sf::Vector2f(
                    static_cast<float>(wp.col * CELL_SIZE),
                    static_cast<float>(wp.row * CELL_SIZE)
                ));
                cell.setFillColor(colorWaypoint);
                // Make waypoints slightly larger for visibility
                cell.setSize(sf::Vector2f(static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)));
                window.draw(cell);
                cell.setSize(sf::Vector2f(static_cast<float>(CELL_SIZE - 1), static_cast<float>(CELL_SIZE - 1)));
            }
        }

        // ========== LAYER 5: Start and Goal (Topmost) ==========
        // Draw Start position
        if (map.isInBounds(start.row, start.col)) {
            cell.setPosition(sf::Vector2f(
                static_cast<float>(start.col * CELL_SIZE),
                static_cast<float>(start.row * CELL_SIZE)
            ));
            cell.setFillColor(colorStart);
            // Make start/goal larger for visibility
            cell.setSize(sf::Vector2f(static_cast<float>(CELL_SIZE + 2), static_cast<float>(CELL_SIZE + 2)));
            window.draw(cell);
        }

        // Draw Goal position
        if (map.isInBounds(goal.row, goal.col)) {
            cell.setPosition(sf::Vector2f(
                static_cast<float>(goal.col * CELL_SIZE),
                static_cast<float>(goal.row * CELL_SIZE)
            ));
            cell.setFillColor(colorGoal);
            cell.setSize(sf::Vector2f(static_cast<float>(CELL_SIZE + 2), static_cast<float>(CELL_SIZE + 2)));
            window.draw(cell);
        }

        // Reset cell size
        cell.setSize(sf::Vector2f(static_cast<float>(CELL_SIZE - 1), static_cast<float>(CELL_SIZE - 1)));

        // Display everything
        window.display();
    }
    
    std::cout << "[GUI] SFML visualizer closed successfully." << std::endl;
#else
    // SFML not enabled - suppress unused parameter warnings
    (void)map;
    (void)path;
    (void)start;
    (void)goal;
    (void)waypoints;
    (void)hazards;
    std::cout << "[GUI] SFML not enabled. To enable GUI:" << std::endl;
    std::cout << "     1. Install SFML library" << std::endl;
    std::cout << "     2. Uncomment #define USE_SFML in gui.cpp or compile with -DUSE_SFML" << std::endl;
    std::cout << "     3. Link against SFML libraries (-lsfml-graphics -lsfml-window -lsfml-system)" << std::endl;
#endif
}

