#ifndef HEAP_H
#define HEAP_H

// Min-heap for A* open set (no STL).
struct HeapNode {
    int vIndex; // flattened vertex index
    int row;
    int col;
    double f;
};

class MinHeap {
private:
    HeapNode* harr;
    int* pos;       // vertex index -> position in heap
    int capacity;
    int heap_size;

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i) const { return (2 * i) + 1; }
    int right(int i) const { return (2 * i) + 2; }
    void swapNode(int i, int j);

public:
    MinHeap(int capacity, int totalVertices);
    ~MinHeap();

    bool isEmpty() const;
    bool contains(int vIndex) const;   // vertex present in heap

    void insertKey(int vIndex, int row, int col, double f);
    HeapNode extractMin();
    void decreaseKey(int vIndex, double newF);
};

#endif // HEAP_H

