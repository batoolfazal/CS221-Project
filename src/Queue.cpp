#include "Queue.h"
#include <iostream>
#include "Queue.h"

Queue::Queue(int size) : frontIndex(0), rearIndex(0), capacity(size), array(0) {
    if (capacity < 1) capacity = 1;
    array = new GridCell[capacity];
}

Queue::~Queue() {
    if (array != 0) {
        delete[] array;
    }
}

bool Queue::isEmpty() const {
    return frontIndex == rearIndex;
}

bool Queue::isFull() const {
    return ((rearIndex + 1) % capacity) == frontIndex;
}

void Queue::enqueue(const GridCell& cell) {
    if (isFull()) {
        return; // silently ignore overflow
    }
    array[rearIndex] = cell;
    rearIndex = (rearIndex + 1) % capacity;
}

GridCell Queue::dequeue() {
    GridCell emptyCell = {-1, -1};
    if (isEmpty()) return emptyCell;
    GridCell value = array[frontIndex];
    frontIndex = (frontIndex + 1) % capacity;
    return value;
}
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
