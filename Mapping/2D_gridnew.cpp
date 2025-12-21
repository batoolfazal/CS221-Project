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

#include "CampusMap.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// ============ REGION STRUCTURE ============
struct Region {
    int r1, c1, r2, c2;
    int type;
    char name[50];
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
        
        // Array-based queue implementation
        int* queue = new int[vertices];
        int front = 0;
        int rear = 0;
        
        queue[rear++] = start;
        visited[start] = true;
        
        while (front < rear) {
            int current = queue[front++];
            
            // Traverse adjacency list
            GraphNode* neighbor = adjList[current];
            while (neighbor != NULL) {
                int neighborVertex = toVertex(neighbor->row, neighbor->col);
                
                if (neighborVertex == goal) {
                    delete[] visited;
                    delete[] queue;
                    return true;
                }
                
                if (!visited[neighborVertex]) {
                    visited[neighborVertex] = true;
                    queue[rear++] = neighborVertex;
                }
                neighbor = neighbor->next;
            }
        }
        
        delete[] visited;
        delete[] queue;
        return false;
    }
    
    int getVertexCount() const { return vertices; }
};

// ============ CAMPUS MAP CLASS ============

    // Initialize region definitions for GIKI campus
    void CampusMap::initializeRegions() {
        regionCount = 0;
        maxRegions = 30;  // Allow some extra space
        regions = new Region[maxRegions];
        
        // Boundary Walls
        Region r1; r1.r1 = 0; r1.c1 = 0; r1.r2 = 2; r1.c2 = 79; r1.type = CELL_BUILDING; strncpy(r1.name, "Top Boundary", 49); r1.name[49] = '\0';
        Region r2; r2.r1 = 0; r2.c1 = 0; r2.r2 = 79; r2.c2 = 2; r2.type = CELL_BUILDING; strncpy(r2.name, "Left Boundary", 49); r2.name[49] = '\0';
        Region r3; r3.r1 = 0; r3.c1 = 77; r3.r2 = 79; r3.c2 = 79; r3.type = CELL_BUILDING; strncpy(r3.name, "Right Boundary", 49); r3.name[49] = '\0';
        Region r4; r4.r1 = 75; r4.c1 = 0; r4.r2 = 78; r4.c2 = 35; r4.type = CELL_BUILDING; strncpy(r4.name, "Bottom Boundary Left", 49); r4.name[49] = '\0';
        Region r5; r5.r1 = 75; r5.c1 = 45; r5.r2 = 78; r5.c2 = 79; r5.type = CELL_BUILDING; strncpy(r5.name, "Bottom Boundary Right", 49); r5.name[49] = '\0';
        
        // No-Fly Zones
        Region r6; r6.r1 = 10; r6.c1 = 35; r6.r2 = 14; r6.c2 = 50; r6.type = CELL_NO_FLY_ZONE; strncpy(r6.name, "Admin Block", 49); r6.name[49] = '\0';
        Region r7; r7.r1 = 29; r7.c1 = 55; r7.r2 = 36; r7.c2 = 70; r7.type = CELL_NO_FLY_ZONE; strncpy(r7.name, "Research Labs", 49); r7.name[49] = '\0';
        Region r8; r8.r1 = 40; r8.c1 = 55; r8.r2 = 45; r8.c2 = 62; r8.type = CELL_NO_FLY_ZONE; strncpy(r8.name, "Server Room", 49); r8.name[49] = '\0';
        
        // Academic Blocks
        Region r9; r9.r1 = 16; r9.c1 = 35; r9.r2 = 26; r9.c2 = 50; r9.type = CELL_BUILDING; strncpy(r9.name, "FES - Engineering Sciences", 49); r9.name[49] = '\0';
        Region r10; r10.r1 = 16; r10.c1 = 55; r10.r2 = 26; r10.c2 = 70; r10.type = CELL_BUILDING; strncpy(r10.name, "FCS - Computer Science", 49); r10.name[49] = '\0';
        Region r11; r11.r1 = 29; r11.c1 = 35; r11.r2 = 36; r11.c2 = 50; r11.type = CELL_BUILDING; strncpy(r11.name, "FME - Mechanical Engineering", 49); r11.name[49] = '\0';
        
        // Hostels
        Region r12; r12.r1 = 36; r12.c1 = 5; r12.r2 = 43; r12.c2 = 15; r12.type = CELL_BUILDING; strncpy(r12.name, "Hostel 1", 49); r12.name[49] = '\0';
        Region r13; r13.r1 = 36; r13.c1 = 18; r13.r2 = 43; r13.c2 = 28; r13.type = CELL_BUILDING; strncpy(r13.name, "Hostel 2", 49); r13.name[49] = '\0';
        Region r14; r14.r1 = 45; r14.c1 = 5; r14.r2 = 52; r14.c2 = 15; r14.type = CELL_BUILDING; strncpy(r14.name, "Hostel 3", 49); r14.name[49] = '\0';
        Region r15; r15.r1 = 45; r15.c1 = 18; r15.r2 = 52; r15.c2 = 28; r15.type = CELL_BUILDING; strncpy(r15.name, "Hostel 4", 49); r15.name[49] = '\0';
        Region r16; r16.r1 = 55; r16.c1 = 5; r16.r2 = 62; r16.c2 = 15; r16.type = CELL_BUILDING; strncpy(r16.name, "Hostel 5", 49); r16.name[49] = '\0';
        Region r17; r17.r1 = 55; r17.c1 = 18; r17.r2 = 62; r17.c2 = 28; r17.type = CELL_BUILDING; strncpy(r17.name, "Hostel 6", 49); r17.name[49] = '\0';
        
        // Other Buildings
        Region r18; r18.r1 = 42; r18.c1 = 35; r18.r2 = 50; r18.c2 = 50; r18.type = CELL_BUILDING; strncpy(r18.name, "Library", 49); r18.name[49] = '\0';
        Region r19; r19.r1 = 50; r19.c1 = 55; r19.r2 = 57; r19.c2 = 65; r19.type = CELL_BUILDING; strncpy(r19.name, "Mosque", 49); r19.name[49] = '\0';
        Region r20; r20.r1 = 58; r20.c1 = 35; r20.r2 = 67; r20.c2 = 50; r20.type = CELL_BUILDING; strncpy(r20.name, "Cafeteria", 49); r20.name[49] = '\0';
        Region r21; r21.r1 = 65; r21.c1 = 55; r21.r2 = 74; r21.c2 = 76; r21.type = CELL_BUILDING; strncpy(r21.name, "Sports Complex", 49); r21.name[49] = '\0';
        
        // Hazards
        Region r22; r22.r1 = 63; r22.c1 = 3; r22.r2 = 66; r22.c2 = 12; r22.type = CELL_HAZARD; strncpy(r22.name, "Construction Zone", 49); r22.name[49] = '\0';
        Region r23; r23.r1 = 69; r23.c1 = 25; r23.r2 = 72; r23.c2 = 30; r23.type = CELL_HAZARD; strncpy(r23.name, "Maintenance Area", 49); r23.name[49] = '\0';
        
        regions[regionCount++] = r1; regions[regionCount++] = r2; regions[regionCount++] = r3;
        regions[regionCount++] = r4; regions[regionCount++] = r5; regions[regionCount++] = r6;
        regions[regionCount++] = r7; regions[regionCount++] = r8; regions[regionCount++] = r9;
        regions[regionCount++] = r10; regions[regionCount++] = r11; regions[regionCount++] = r12;
        regions[regionCount++] = r13; regions[regionCount++] = r14; regions[regionCount++] = r15;
        regions[regionCount++] = r16; regions[regionCount++] = r17; regions[regionCount++] = r18;
        regions[regionCount++] = r19; regions[regionCount++] = r20; regions[regionCount++] = r21;
        regions[regionCount++] = r22; regions[regionCount++] = r23;
    }

    // Robust boundary-checked region filling
    void CampusMap::fillRegion(int r1, int c1, int r2, int c2, int cellType) {
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
    void CampusMap::buildGraph() {
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

    // Constructor
    CampusMap::CampusMap(int r, int c) : rows(r), cols(c), campusGraph(NULL), regionCount(0), maxRegions(30) {
        // Allocate 2D array
        grid = new int*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new int[cols];
            for (int j = 0; j < cols; j++) {
                grid[i][j] = CELL_FREE_PATH;
            }
        }
        regions = NULL;
        initializeRegions();
    }

    // Destructor
    CampusMap::~CampusMap() {
        // Deallocate 2D array
        if (grid != NULL) {
            for (int i = 0; i < rows; i++) {
                delete[] grid[i];
            }
            delete[] grid;
        }
        if (regions != NULL) {
            delete[] regions;
        }
        if (campusGraph != NULL) {
            delete campusGraph;
        }
    }

    // Getters
    int CampusMap::getRows() const { return rows; }
    int CampusMap::getCols() const { return cols; }
    
    // Check if cell is within bounds
    bool CampusMap::isInBounds(int row, int col) const {
        return (row >= 0 && row < rows && col >= 0 && col < cols);
    }
    
    // Check if cell is free path
    bool CampusMap::isFree(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_FREE_PATH;
    }
    
    // Check if cell is obstacle
    bool CampusMap::isObstacle(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_BUILDING;
    }
    
    // Check if cell is no-fly zone
    bool CampusMap::isNoFlyZone(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_NO_FLY_ZONE;
    }

    // Check if cell is hazard
    bool CampusMap::isHazard(int row, int col) const {
        return isInBounds(row, col) && grid[row][col] == CELL_HAZARD;
    }

    // Mutators for mapping
    void CampusMap::addWall(int row, int col) {
        if (isInBounds(row, col)) {
            grid[row][col] = CELL_BUILDING;
        }
    }

    void CampusMap::removeWall(int row, int col) {
        if (isInBounds(row, col)) {
            grid[row][col] = CELL_FREE_PATH;
        }
    }

    bool CampusMap::isBlocked(int row, int col) const {
        return isObstacle(row, col) || isNoFlyZone(row, col) || isHazard(row, col);
    }

    // Get cell value
    int CampusMap::getCell(int row, int col) const {
        if (!isInBounds(row, col)) return -1;
        return grid[row][col];
    }

    // Build the campus map from regions
    void CampusMap::buildMap() {
        // Reset grid to free paths
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = CELL_FREE_PATH;
            }
        }
        
        // Fill all defined regions
        for (int i = 0; i < regionCount; i++) {
            fillRegion(regions[i].r1, regions[i].c1, 
                      regions[i].r2, regions[i].c2, regions[i].type);
        }
        
        // Build graph representation
        buildGraph();
    }

    // BFS path check using Graph ADT
    bool CampusMap::isPathPossible(int startR, int startC, int goalR, int goalC) {
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

    // Load grid from file
    bool CampusMap::loadFromFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << endl;
            return false;
        }
        
        int fileRows, fileCols;
        if (!(file >> fileRows >> fileCols)) {
            cout << "Error: Failed to read grid dimensions from " << filename << endl;
            file.close();
            return false;
        }
        
        // Check if dimensions match
        if (fileRows != rows || fileCols != cols) {
            cout << "Warning: File dimensions (" << fileRows << "x" << fileCols 
                 << ") don't match grid (" << rows << "x" << cols << ")" << endl;
            cout << "Resizing grid to match file..." << endl;
            
            // Deallocate old grid
            for (int i = 0; i < rows; i++) {
                delete[] grid[i];
            }
            delete[] grid;
            
            // Allocate new grid
            rows = fileRows;
            cols = fileCols;
            grid = new int*[rows];
            for (int i = 0; i < rows; i++) {
                grid[i] = new int[cols];
            }
        }
        
        // Read grid data
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (!(file >> grid[i][j])) {
                    cout << "Error: Failed to read cell at (" << i << "," << j << ")" << endl;
                    file.close();
                    return false;
                }
            }
        }
        
        file.close();
        cout << "Map loaded from " << filename << " (" << rows << "x" << cols << ")" << endl;
        
        // Rebuild graph after loading
        buildGraph();
        
        return true;
    }

    // Save grid to file
    bool CampusMap::saveToFile(const char* filename) {
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
    void CampusMap::visualize() const {
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
    void CampusMap::printStats() const {
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

