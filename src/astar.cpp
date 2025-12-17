#include "astar.h"

// ======================= Supporting Functions =======================
bool isValid(int x, int y, GridMap& map, HazardDetector& detector) {
    if (map.isObstacle(x, y)) return false;
    if (map.isNoFlyZone(x, y)) return false;
    if (detector.isHazard(x, y)) return false;
    return true;
}

double calculateH(int row, int col, int destRow, int destCol) {
    return sqrt((row - destRow)*(row - destRow) + (col - destCol)*(col - destCol));
}

bool isDestination(int row, int col, int destRow, int destCol) {
    return (row == destRow && col == destCol);
}

void tracePath(Cell** cellDetails, int destRow, int destCol) {
    stack<pair<int,int>> Path;
    int row = destRow;
    int col = destCol;

    while (!(cellDetails[row][col].parentRow == row &&
             cellDetails[row][col].parentCol == col)) {
        Path.push(make_pair(row, col));
        int tempRow = cellDetails[row][col].parentRow;
        int tempCol = cellDetails[row][col].parentCol;
        row = tempRow;
        col = tempCol;
    }
    Path.push(make_pair(row, col));

    cout << "Path: ";
    while (!Path.empty()) {
        pair<int,int> p = Path.top();
        Path.pop();
        cout << "(" << p.first << "," << p.second << ") ";
    }
    cout << endl;
}

// ======================= A* Algorithm =======================
void aStarSearch(GridMap& map, HazardDetector& detector,
                 int ROW, int COL, int srcRow, int srcCol,
                 int destRow, int destCol) {

    if (!isValid(srcRow, srcCol, map, detector) ||
        !isValid(destRow, destCol, map, detector)) {
        cout << "Source or Destination is invalid!" << endl;
        return;
    }

    if (isDestination(srcRow, srcCol, destRow, destCol)) {
        cout << "Already at Destination!" << endl;
        return;
    }

    // Closed list
    bool** closedList = new bool*[ROW];
    for (int i = 0; i < ROW; i++) closedList[i] = new bool[COL]{false};

    // Cell details
    Cell** cellDetails = new Cell*[ROW];
    for (int i = 0; i < ROW; i++) {
        cellDetails[i] = new Cell[COL];
        for (int j = 0; j < COL; j++) {
            cellDetails[i][j].f = numeric_limits<double>::max();
            cellDetails[i][j].g = numeric_limits<double>::max();
            cellDetails[i][j].h = numeric_limits<double>::max();
            cellDetails[i][j].parentRow = -1;
            cellDetails[i][j].parentCol = -1;
        }
    }

    // Initialize source cell
    cellDetails[srcRow][srcCol].f = 0.0;
    cellDetails[srcRow][srcCol].g = 0.0;
    cellDetails[srcRow][srcCol].h = 0.0;
    cellDetails[srcRow][srcCol].parentRow = srcRow;
    cellDetails[srcRow][srcCol].parentCol = srcCol;

    // Open list (simple array for now)
    int openListSize = ROW * COL;
    pair<double, pair<int,int>>* openList = new pair<double, pair<int,int>>[openListSize];
    int openCount = 1;
    openList[0] = make_pair(0.0, make_pair(srcRow, srcCol));

    bool foundDest = false;

    // Directions: N, S, E, W, NE, NW, SE, SW
    int rowDir[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int colDir[] = {0, 0, 1, -1, 1, -1, 1, -1};
    double moveCost[] = {1.0,1.0,1.0,1.0,1.414,1.414,1.414,1.414};

    while (openCount > 0) {
        // Find cell with minimum f
        int minIndex = 0;
        for (int k = 1; k < openCount; k++)
            if (openList[k].first < openList[minIndex].first)
                minIndex = k;

        pair<int,int> current = openList[minIndex].second;

        // Remove from open list
        openCount--;
        for (int k = minIndex; k < openCount; k++)
            openList[k] = openList[k+1];

        int i = current.first;
        int j = current.second;
        closedList[i][j] = true;

        // Check all 8 neighbors
        for (int dir = 0; dir < 8; dir++) {
            int newRow = i + rowDir[dir];
            int newCol = j + colDir[dir];

            if (isValid(newRow, newCol, map, detector)) {
                if (isDestination(newRow, newCol, destRow, destCol)) {
                    cellDetails[newRow][newCol].parentRow = i;
                    cellDetails[newRow][newCol].parentCol = j;
                    cout << "Destination found!" << endl;
                    tracePath(cellDetails, destRow, destCol);
                    foundDest = true;
                    return;
                }

                if (!closedList[newRow][newCol]) {
                    double gNew = cellDetails[i][j].g + moveCost[dir];
                    double hNew = calculateH(newRow, newCol, destRow, destCol);
                    double fNew = gNew + hNew;

                    if (cellDetails[newRow][newCol].f == numeric_limits<double>::max() || cellDetails[newRow][newCol].f > fNew) {
                        openList[openCount++] = make_pair(fNew, make_pair(newRow, newCol));
                        cellDetails[newRow][newCol].f = fNew;
                        cellDetails[newRow][newCol].g = gNew;
                        cellDetails[newRow][newCol].h = hNew;
                        cellDetails[newRow][newCol].parentRow = i;
                        cellDetails[newRow][newCol].parentCol = j;
                    }
                }
            }
        }
    }

    if (!foundDest) cout << "Failed to find Destination\n";
}

