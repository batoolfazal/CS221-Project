#include "GridMap.h"

GridMap::GridMap(int r, int c) : rows(0), cols(0), cells(0) {
    if (r > 0 && c > 0) {
        allocate(r, c);
    }
}

GridMap::~GridMap() {
    release();
}

void GridMap::allocate(int r, int c) {
    rows = r;
    cols = c;
    cells = new int*[rows];
    for (int i = 0; i < rows; i++) {
        cells[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            cells[i][j] = 0; // default free
        }
    }
}

void GridMap::release() {
    if (cells != 0) {
        for (int i = 0; i < rows; i++) {
            delete[] cells[i];
        }
        delete[] cells;
        cells = 0;
    }
    rows = 0;
    cols = 0;
}

int GridMap::getRows() const { return rows; }
int GridMap::getCols() const { return cols; }

bool GridMap::isInBounds(int r, int c) const {
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

bool GridMap::isFree(int r, int c) const {
    return isInBounds(r, c) && cells[r][c] == 0;
}

bool GridMap::isObstacle(int r, int c) const {
    return isInBounds(r, c) && cells[r][c] == 1;
}

bool GridMap::isNoFlyZone(int r, int c) const {
    return isInBounds(r, c) && cells[r][c] == 2;
}

void GridMap::setCell(int r, int c, int value) {
    if (isInBounds(r, c)) {
        cells[r][c] = value;
    }
}

HazardDetector::HazardDetector(int r, int c) : rows(0), cols(0), hazard(0) {
    if (r > 0 && c > 0) {
        allocate(r, c);
    }
}

HazardDetector::~HazardDetector() {
    release();
}

void HazardDetector::allocate(int r, int c) {
    rows = r;
    cols = c;
    hazard = new bool*[rows];
    for (int i = 0; i < rows; i++) {
        hazard[i] = new bool[cols];
        for (int j = 0; j < cols; j++) {
            hazard[i][j] = false;
        }
    }
}

void HazardDetector::release() {
    if (hazard != 0) {
        for (int i = 0; i < rows; i++) {
            delete[] hazard[i];
        }
        delete[] hazard;
        hazard = 0;
    }
    rows = 0;
    cols = 0;
}

void HazardDetector::setHazard(int r, int c, bool present) {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        hazard[r][c] = present;
    }
}

bool HazardDetector::isHazard(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    return hazard[r][c];
}
