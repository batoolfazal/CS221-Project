#include <SFML/Graphics.hpp>

/**
 * @brief Renders the mission map, NFZs, and the final telemetry path.
 * @param map The CampusMap object containing grid data.
 * @param pathArr The array of TelemetryEntry points (the path).
 * @param pathSize The number of steps in the path.
 */
void displayMissionGUI(CampusMap& map, TelemetryEntry* pathArr, int pathSize) {
    const int CELL_SIZE = 10; // Scale 80x80 to 800x800
    sf::RenderWindow window(sf::VideoMode(800, 800), "Mission Orchestrator - SFML Visualizer");

    // Colors
    sf::Color colorWall(40, 40, 40);      // Dark Grey
    sf::Color colorFree(220, 220, 220);   // Light Grey
    sf::Color colorNFZ(255, 0, 0, 150);   // Red (Transparent)
    sf::Color colorPath(0, 150, 255);     // Blue

    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1.0f, CELL_SIZE - 1.0f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // 1. Draw the Base Map and NFZs
        for (int r = 0; r < map.getRows(); r++) {
            for (int c = 0; c < map.getCols(); c++) {
                cell.setPosition(c * CELL_SIZE, r * CELL_SIZE);

                if (!map.isFree(r, c)) {
                    cell.setFillColor(colorWall);
                } else if (isNFZ(r, c)) {
                    cell.setFillColor(colorNFZ);
                } else {
                    cell.setFillColor(colorFree);
                }
                window.draw(cell);
            }
        }

        // 2. Draw the Path (Telemetry)
        cell.setFillColor(colorPath);
        for (int i = 0; i < pathSize; i++) {
            // Mapping path row/col to window x/y
            cell.setPosition(pathArr[i].col * CELL_SIZE, pathArr[i].row * CELL_SIZE);
            window.draw(cell);
        }

        window.display();
    }
}
// ... [After Step 8: telemetry.toArray] ...

    // 9. Visualization
    std::cout << "Opening Mission GUI..." << std::endl;
    displayMissionGUI(campus, arr, n);

    // 10. Report generation ...
