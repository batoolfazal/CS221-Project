#include "gui.h"

// #ifdef USE_SFML
// #include <SFML/Graphics.hpp>
// #endif

void displayMissionGUI(
    CampusMap& map,
    const std::vector<TelemetryEntry>& path,
    const Waypoint& start,
    const Waypoint& goal,
    const std::vector<Waypoint>& waypoints,
    const std::vector<Hazard>& hazards
) {
#ifdef USE_SFML
    const int CELL_SIZE = 10; // Scale 80x80 -> 800x800
    sf::RenderWindow window(sf::VideoMode(map.getCols() * CELL_SIZE, map.getRows() * CELL_SIZE),
                            "Mission Orchestrator - SFML Visualizer");

    // Colors
    sf::Color colorWall(40, 40, 40);           // Dark Grey
    sf::Color colorFree(220, 220, 220);        // Light Grey
    sf::Color colorNFZ(255, 0, 0, 150);        // Red transparent
    sf::Color colorPath(0, 150, 255);          // Blue
    sf::Color colorStart(0, 255, 0);           // Green
    sf::Color colorGoal(255, 165, 0);          // Orange
    sf::Color colorWaypoint(255, 255, 0);      // Yellow
    sf::Color colorHazard(255, 0, 255);        // Magenta

    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1.f, CELL_SIZE - 1.f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw base map
        for (int r = 0; r < map.getRows(); r++) {
            for (int c = 0; c < map.getCols(); c++) {
                cell.setPosition(c * CELL_SIZE, r * CELL_SIZE);
                if (!map.isFree(r, c)) cell.setFillColor(colorWall);
                else if (map.isNoFlyZone(r, c) || isNFZ(r, c)) cell.setFillColor(colorNFZ);
                else cell.setFillColor(colorFree);
                window.draw(cell);
            }
        }

        // Draw hazards
        for (const auto& h : hazards) {
            cell.setPosition(h.col * CELL_SIZE, h.row * CELL_SIZE);
            cell.setFillColor(colorHazard);
            window.draw(cell);
        }

        // Draw path
        for (const auto& step : path) {
            cell.setPosition(step.col * CELL_SIZE, step.row * CELL_SIZE);
            cell.setFillColor(colorPath);
            window.draw(cell);
        }

        // Draw waypoints
        for (const auto& wp : waypoints) {
            cell.setPosition(wp.col * CELL_SIZE, wp.row * CELL_SIZE);
            cell.setFillColor(colorWaypoint);
            window.draw(cell);
        }

        // Draw Start & Goal
        cell.setPosition(start.col * CELL_SIZE, start.row * CELL_SIZE);
        cell.setFillColor(colorStart);
        window.draw(cell);

        cell.setPosition(goal.col * CELL_SIZE, goal.row * CELL_SIZE);
        cell.setFillColor(colorGoal);
        window.draw(cell);

        window.display();
    }
#else
    (void)map; (void)path; (void)start; (void)goal; (void)waypoints; (void)hazards;
    std::cout << "[GUI] SFML not enabled (define USE_SFML and link SFML to use GUI).\n";
#endif
}

