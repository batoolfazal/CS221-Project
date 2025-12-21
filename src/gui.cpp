#include "gui.h"
#include "NFZ.h"

// #define USE_SFML
#include <SFML/Graphics.hpp>

void displayMissionGUI(
    CampusMap& map,
    const std::vector<TelemetryEntry>& path,
    const Waypoint& start,
    const Waypoint& goal,
    const std::vector<Waypoint>& waypoints,
    const std::vector<Hazard>& hazards
) {
#ifdef USE_SFML
    std::cout << "Opening SFML GUI window..." << std::endl;
    const int CELL_SIZE = 5; // Scale 80x80 -> 400x400
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(map.getCols() * CELL_SIZE), static_cast<unsigned int>(map.getRows() * CELL_SIZE)}),
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
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw base map
        for (int r = 0; r < map.getRows(); r++) {
            for (int c = 0; c < map.getCols(); c++) {
                cell.setPosition(sf::Vector2f(c * CELL_SIZE, r * CELL_SIZE));
                if (!map.isFree(r, c)) cell.setFillColor(colorWall);
                else if (map.isNoFlyZone(r, c) || isNFZ(r, c)) cell.setFillColor(colorNFZ);
                else cell.setFillColor(colorFree);
                window.draw(cell);
            }
        }

        // Draw hazards
        for (const auto& h : hazards) {
            cell.setPosition(sf::Vector2f(h.x * CELL_SIZE, h.y * CELL_SIZE));
            cell.setFillColor(colorHazard);
            window.draw(cell);
        }

        // Draw path
        for (const auto& step : path) {
            cell.setPosition(sf::Vector2f(step.col * CELL_SIZE, step.row * CELL_SIZE));
            cell.setFillColor(colorPath);
            window.draw(cell);
        }

        // Draw waypoints
        for (const auto& wp : waypoints) {
            cell.setPosition(sf::Vector2f(wp.col * CELL_SIZE, wp.row * CELL_SIZE));
            cell.setFillColor(colorWaypoint);
            window.draw(cell);
        }

        // Draw Start & Goal
        cell.setPosition(sf::Vector2f(start.col * CELL_SIZE, start.row * CELL_SIZE));
        cell.setFillColor(colorStart);
        window.draw(cell);

        cell.setPosition(sf::Vector2f(goal.col * CELL_SIZE, goal.row * CELL_SIZE));
        cell.setFillColor(colorGoal);
        window.draw(cell);

        window.display();
    }
#else
    (void)map; (void)path; (void)start; (void)goal; (void)waypoints; (void)hazards;
    std::cout << "[GUI] SFML not enabled (define USE_SFML and link SFML to use GUI).\n";
#endif
}

