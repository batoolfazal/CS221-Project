#ifndef QUEUE_H
#define QUEUE_H

using namespace std;

// Represents a single cell in the grid
struct GridCell {
    int row; // Row index
    int col; // Column index
};

// Simple array-based queue for BFS
class Queue {
private:
    int frontIndex, rearIndex, capacity;
    GridCell* array; // Dynamic array to store queue elements

public:
    Queue(int size);   // Constructor with fixed size
    ~Queue();          // Destructor to free memory

    bool isEmpty() const;  // Check if queue is empty
    bool isFull() const;   // Check if queue is full

    void enqueue(const GridCell& cell); // Add cell to queue
    GridCell dequeue();                  // Remove cell from queue
};

#endif
