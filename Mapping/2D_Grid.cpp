/**
 * GIKI Campus 2D Grid Environment - Professional Map Generator
 * 
 * This module generates a 2D grid representation of GIKI campus.
 * Each cell = 10m x 10m, Grid size = 80x80 (800m x 800m campus)
 * 
 * Features:
 *   - Encapsulated CampusMap class with dynamic memory
 *   - Graph ADT representation (adjacency list)
 *   - Enum class for type-safe cell types
 *   - Region-based structure initialization
 *   - BFS-based path connectivity verification using Graph
 *   - Robust boundary checking
 * 
 * DSA Course Project - GIKI Institute
 */

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// ============ CELL TYPE CONSTANTS ============
const int CELL_FREE_PATH = 0;
const int CELL_BUILDING = 1;
const int CELL_NO_FLY_ZONE = 2;
const int CELL_HAZARD = 3;

// ============ REGION STRUCTURE ============
struct Region {
    int r1, c1, r2, c2;
    int type;
    string name;
};

// ============ GRAPH NODE ============
struct GraphNode {
    int row, col;
    GraphNode* next;
    
    GraphNode(int r, int c) : row(r), col(c), next(NULL) {}
};

// ============ GRAPH ADT CLASS ============
class Graph {
private:
    int vertices;
    int rows, cols;
    GraphNode** adjList;  // Adjacency list array
    
    // Convert 2D coordinates to 1D vertex index
    int toVertex(int row, int col) const {
        return row * cols + col;
    }
    
    // Convert 1D vertex to row
    int toRow(int vertex) const {
        return vertex / cols;
    }
    
    // Convert 1D vertex to column
    int toCol(int vertex) const {
        return vertex % cols;
    }

public:
    Graph(int r, int c) : rows(r), cols(c) {
        vertices = rows * cols;
        adjList = new GraphNode*[vertices];
        for (int i = 0; i < vertices; i++) {
            adjList[i] = NULL;
        }
    }
    
    ~Graph() {
        for (int i = 0; i < vertices; i++) {
            GraphNode* current = adjList[i];
            while (current != NULL) {
                GraphNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] adjList;
    }
    
    // Add edge between two cells
    void addEdge(int r1, int c1, int r2, int c2) {
        int v1 = toVertex(r1, c1);
        int v2 = toVertex(r2, c2);
        
        // Add v2 to v1's list
        GraphNode* newNode = new GraphNode(r2, c2);
        newNode->next = adjList[v1];
        adjList[v1] = newNode;
    }
    
    // BFS to find path between two cells
    bool bfsPathExists(int startR, int startC, int goalR, int goalC) {
        int start = toVertex(startR, startC);
        int goal = toVertex(goalR, goalC);
        
        if (start == goal) return true;
        
        bool* visited = new bool[vertices];
        for (int i = 0; i < vertices; i++) {
            visited[i] = false;
        }
        
        int* bfsQueue = new int[vertices];
        int front = 0;
        int back = 0;
        bfsQueue[back++] = start;
        visited[start] = true;
        
        while (front < back) {
            int current = bfsQueue[front++];
            
            // Traverse adjacency list
            GraphNode* neighbor = adjList[current];
            while (neighbor != NULL) {
                int neighborVertex = toVertex(neighbor->row, neighbor->col);
                
                if (neighborVertex == goal) {
                    delete[] visited;
                    delete[] bfsQueue;
                    return true;
                }
                
                if (!visited[neighborVertex]) {
                    visited[neighborVertex] = true;
                    bfsQueue[back++] = neighborVertex;
                }
                neighbor = neighbor->next;
            }
        }
        
        delete[] visited;
        delete[] bfsQueue;
        return false;
    }
    
    int getVertexCount() const { return vertices; }
};

// ============ CAMPUS MAP CLASS ============
class CampusMap {
private:
    int rows;
    int cols;
    int** grid;
    Region* regions;
    int regionCount;
    Graph* campusGraph;
    
    static const int MAX_REGION_CAP = 32;

    // Initialize region definitions for GIKI campus
    void initializeRegions() {
        regions = new Region[MAX_REGION_CAP];
        regionCount = 0;

        setRegion(0, 0, 0, 2, 79, CELL_BUILDING, "Top Boundary");
        setRegion(1, 0, 0, 79, 2, CELL_BUILDING, "Left Boundary");
        setRegion(2, 0, 77, 79, 79, CELL_BUILDING, "Right Boundary");
        setRegion(3, 75, 0, 78, 35, CELL_BUILDING, "Bottom Boundary Left");
        setRegion(4, 75, 45, 78, 79, CELL_BUILDING, "Bottom Boundary Right");

        setRegion(5, 10, 35, 14, 50, CELL_NO_FLY_ZONE, "Admin Block");
        setRegion(6, 29, 55, 36, 70, CELL_NO_FLY_ZONE, "Research Labs");
        setRegion(7, 40, 55, 45, 62, CELL_NO_FLY_ZONE, "Server Room");

        setRegion(8, 16, 35, 26, 50, CELL_BUILDING, "FES - Engineering Sciences");
        setRegion(9, 16, 55, 26, 70, CELL_BUILDING, "FCS - Computer Science");
        setRegion(10, 29, 35, 36, 50, CELL_BUILDING, "FME - Mechanical Engineering");

        setRegion(11, 36, 5, 43, 15, CELL_BUILDING, "Hostel 1");
        setRegion(12, 36, 18, 43, 28, CELL_BUILDING, "Hostel 2");
        setRegion(13, 45, 5, 52, 15, CELL_BUILDING, "Hostel 3");
        setRegion(14, 45, 18, 52, 28, CELL_BUILDING, "Hostel 4");
        setRegion(15, 55, 5, 62, 15, CELL_BUILDING, "Hostel 5");
        setRegion(16, 55, 18, 62, 28, CELL_BUILDING, "Hostel 6");

        setRegion(17, 42, 35, 50, 50, CELL_BUILDING, "Library");
        setRegion(18, 50, 55, 57, 65, CELL_BUILDING, "Mosque");
        setRegion(19, 58, 35, 67, 50, CELL_BUILDING, "Cafeteria");
        setRegion(20, 65, 55, 74, 76, CELL_BUILDING, "Sports Complex");

        setRegion(21, 63, 3, 66, 12, CELL_HAZARD, "Construction Zone");
        setRegion(22, 69, 25, 72, 30, CELL_HAZARD, "Maintenance Area");

        regionCount = 23;
    }

    void setRegion(int index, int r1, int c1, int r2, int c2, int type, const char* nameText) {
        regions[index].r1 = r1;
        regions[index].c1 = c1;
        regions[index].r2 = r2;
        regions[index].c2 = c2;
        regions[index].type = type;
        std::strncpy(regions[index].name, nameText, sizeof(regions[index].name) - 1);
        regions[index].name[sizeof(regions[index].name) - 1] = '\0';
    }

    void allocateGrid(int r, int c) {
        rows = r;
        cols = c;
        grid = new int*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new int[cols];
        }
    }

    void releaseGrid() {
        if (grid != NULL) {
            for (int i = 0; i < rows; i++) {
                delete[] grid[i];
            }
            delete[] grid;
            grid = NULL;
        }
    }

    void resetGridToFree() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = CELL_FREE_PATH;
            }
        }
    }

    // Robust boundary-checked region filling
    void fillRegion(int r1, int c1, int r2, int c2, int cellType) {
        int startRow = (r1 < 0) ? 0 : (r1 >= rows ? rows - 1 : r1);
        int endRow = (r2 < 0) ? 0 : (r2 >= rows ? rows - 1 : r2);
        int startCol = (c1 < 0) ? 0 : (c1 >= cols ? cols - 1 : c1);
        int endCol = (c2 < 0) ? 0 : (c2 >= cols ? cols - 1 : c2);
        
        for (int i = startRow; i <= endRow; i++) {
            for (int j = startCol; j <= endCol; j++) {
                grid[i][j] = cellType;
            }
        }
    }

    // Build graph from grid (connect adjacent free cells)
    void buildGraph() {
        if (campusGraph != NULL) {
            delete campusGraph;
        }
        campusGraph = new Graph(rows, cols);
        
        // Direction arrays (4-directional: up, down, left, right)
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (isFree(i, j)) {
                    // Add edges to adjacent free cells
                    for (int d = 0; d < 4; d++) {
                        int ni = i + dr[d];
                        int nj = j + dc[d];
                        if (isFree(ni, nj)) {
                            campusGraph->addEdge(i, j, ni, nj);
                        }
                    }
                }
            }
        }
    }

public:
    // Constructor
    CampusMap(int r = 80, int c = 80) : rows(r), cols(c), grid(NULL), regions(NULL), regionCount(0), campusGraph(NULL) {
        allocateGrid(rows, cols);
        initializeRegions();
        resetGridToFree();
    }

    // Destructor
    ~CampusMap() {
        releaseGrid();
        if (campusGraph != NULL) {
            delete campusGraph;
        }
        if (regions != NULL) {
            delete[] regions;
        }
    }

    // Getters
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    
    // Check if cell is within bounds
    bool isInBounds(int row, int col) const {
        return (row >= 0 && row < rows && col >= 0 && col < cols);
    }
    
    // Check if cell is free path
    bool isFree(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_FREE_PATH;
    }
    
    // Check if cell is obstacle
    bool isObstacle(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_BUILDING;
    }
    
    // Check if cell is no-fly zone
    bool isNoFlyZone(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_NO_FLY_ZONE;
    }

    // Get cell value
    int getCell(int row, int col) const {
        if (!isInBounds(row, col)) return -1;
        return grid[row][col];
    }

    // Build the campus map from regions
    void buildMap() {
        // Reset grid to free paths
        resetGridToFree();
        
        // Fill all defined regions
        for (int i = 0; i < regionCount; i++) {
            fillRegion(regions[i].r1, regions[i].c1, 
                      regions[i].r2, regions[i].c2, regions[i].type);
        }
        
        // Build graph representation
        buildGraph();
    }

    // Load an existing map file (rows cols followed by grid values)
    bool loadFromFile(const char* filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Cannot open map file " << filename << std::endl;
            return false;
        }

        int fileRows = 0;
        int fileCols = 0;
        if (!(file >> fileRows >> fileCols)) {
            std::cout << "Error: Invalid map header in " << filename << std::endl;
            file.close();
            return false;
        }

        if (fileRows <= 0 || fileCols <= 0) {
            std::cout << "Error: Invalid map dimensions in " << filename << std::endl;
            file.close();
            return false;
        }

        if (fileRows != rows || fileCols != cols) {
            releaseGrid();
            allocateGrid(fileRows, fileCols);
        }

        for (int i = 0; i < fileRows; i++) {
            for (int j = 0; j < fileCols; j++) {
                if (!(file >> grid[i][j])) {
                    std::cout << "Error: Incomplete map data in " << filename << std::endl;
                    file.close();
                    return false;
                }
            }
        }

        file.close();
        buildGraph();
        return true;
    }

    // BFS path check using Graph ADT
    bool isPathPossible(int startR, int startC, int goalR, int goalC) {
        if (!isFree(startR, startC)) {
            cout << "Error: Start (" << startR << "," << startC << ") not free!" << endl;
            return false;
        }
        if (!isFree(goalR, goalC)) {
            cout << "Error: Goal (" << goalR << "," << goalC << ") not free!" << endl;
            return false;
        }
        
        if (campusGraph == NULL) {
            cout << "Error: Graph not built. Call buildMap() first." << endl;
            return false;
        }
        
        return campusGraph->bfsPathExists(startR, startC, goalR, goalC);
    }

    // Save grid to file
    bool saveToFile(const char* filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot create file " << filename << endl;
            return false;
        }
        
        file << rows << " " << cols << "\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                file << grid[i][j];
                if (j < cols - 1) file << " ";
            }
            file << "\n";
        }
        
        file.close();
        cout << "Map saved to " << filename << endl;
        return true;
    }

    // Console visualization
    void visualize() const {
        cout << "\n========== GIKI CAMPUS MAP ==========" << endl;
        cout << "Legend: . = Path, # = Building, N = No-Fly, H = Hazard" << endl;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char symbol;
                switch (grid[i][j]) {
                    case 0: symbol = '.'; break;
                    case 1: symbol = '#'; break;
                    case 2: symbol = 'N'; break;
                    case 3: symbol = 'H'; break;
                    default: symbol = '?'; break;
                }
                cout << symbol;
            }
            cout << endl;
        }
        cout << "======================================" << endl;
    }

    // Print statistics
    void printStats() const {
        int freeCells = 0, buildings = 0, noFly = 0, hazards = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                switch (grid[i][j]) {
                    case 0: freeCells++; break;
                    case 1: buildings++; break;
                    case 2: noFly++; break;
                    case 3: hazards++; break;
                }
            }
        }
        cout << "\nStatistics:" << endl;
        cout << "- Free: " << freeCells << ", Buildings: " << buildings;
        cout << ", No-Fly: " << noFly << ", Hazards: " << hazards << endl;
    }
};

// ============ MAIN ============
int main() {
    ios_base::sync_with_stdio(false);
    
    cout << "=== GIKI Campus Map Loader ===" << endl;
    
    CampusMap campus(80, 80);

    if (campus.loadFromFile("Mapping/campus_map.txt")) {
        cout << "Loaded map from Mapping/campus_map.txt" << endl;
    } else {
        cout << "Generating map from region definitions..." << endl;
        campus.buildMap();
        campus.saveToFile("Mapping/campus_map.txt");
    }

    campus.visualize();
    campus.printStats();
    
    // Path connectivity test using Graph ADT + BFS
    cout << "\n=== Graph-Based Path Test ===" << endl;
    int startR = 74, startC = 40;  // Main Gate
    int goalR = 15, goalC = 34;    // Academic Block
    
    cout << "Path from Gate(" << startR << "," << startC << ") to Academic(" << goalR << "," << goalC << "): ";
    cout << (campus.isPathPossible(startR, startC, goalR, goalC) ? "EXISTS" : "BLOCKED") << endl;
    
    return 0;
}
