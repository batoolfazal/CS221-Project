#include "Queue.h"
#include <iostream>

using namespace std;

Queue::Queue(int size) {
    capacity = size;
    array = new GridCell[capacity]; // Allocate array
    frontIndex = 0;                  // Initialize front
    rearIndex = 0;                   // Initialize rear
}

Queue::~Queue() {
    delete[] array; // Free allocated memory
}

bool Queue::isEmpty() const {
    return frontIndex == rearIndex; // No elements
}

bool Queue::isFull() const {
    return rearIndex == capacity;   // Array is full
}

// Add a cell to the rear of the queue
void Queue::enqueue(const GridCell& cell) {
    if (isFull()) {
        cout << "Queue overflow!\n";
        return;
    }
    array[rearIndex++] = cell;
}

// Remove and return the cell at the front
GridCell Queue::dequeue() {
    if (isEmpty()) {
        cout << "Queue underflow!\n";
        return {-1, -1}; // Invalid cell
    }
    return array[frontIndex++];
}
