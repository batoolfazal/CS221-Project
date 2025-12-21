#include "heap.h"

MinHeap::MinHeap(int cap, int totalVertices) : capacity(cap), heap_size(0) {
    if (capacity < 1) capacity = 1;
    harr = new HeapNode[capacity];
    pos = new int[totalVertices];
    for (int i = 0; i < totalVertices; i++) pos[i] = -1;
}

MinHeap::~MinHeap() {
    if (harr != 0) delete[] harr;
    if (pos != 0) delete[] pos;
}

bool MinHeap::isEmpty() const {
    return heap_size == 0;
}

bool MinHeap::contains(int vIndex) const {
    if (vIndex < 0) return false;
    return pos[vIndex] != -1 && pos[vIndex] < heap_size;
}

void MinHeap::swapNode(int i, int j) {
    HeapNode tmp = harr[i];
    harr[i] = harr[j];
    harr[j] = tmp;
    pos[harr[i].vIndex] = i;
    pos[harr[j].vIndex] = j;
}

void MinHeap::insertKey(int vIndex, int row, int col, double f) {
    if (heap_size == capacity) return;
    int i = heap_size++;
    harr[i].vIndex = vIndex;
    harr[i].row = row;
    harr[i].col = col;
    harr[i].f = f;
    pos[vIndex] = i;

    // Fix min-heap property
    while (i != 0 && harr[parent(i)].f > harr[i].f) {
        swapNode(i, parent(i));
        i = parent(i);
    }
}

HeapNode MinHeap::extractMin() {
    HeapNode empty = {-1, -1, -1, 0};
    if (heap_size <= 0) return empty;
    if (heap_size == 1) {
        heap_size--;
        pos[harr[0].vIndex] = -1;
        return harr[0];
    }

    HeapNode root = harr[0];
    harr[0] = harr[heap_size - 1];
    pos[harr[0].vIndex] = 0;
    pos[root.vIndex] = -1;
    heap_size--;

    int i = 0;
    while (true) {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        if (l < heap_size && harr[l].f < harr[smallest].f) smallest = l;
        if (r < heap_size && harr[r].f < harr[smallest].f) smallest = r;

        if (smallest != i) {
            swapNode(i, smallest);
            i = smallest;
        } else {
            break;
        }
    }

    return root;
}

void MinHeap::decreaseKey(int vIndex, double newF) {
    int i = pos[vIndex];
    if (i == -1 || i >= heap_size) return;
    if (harr[i].f <= newF) return;

    harr[i].f = newF;
    while (i != 0 && harr[parent(i)].f > harr[i].f) {
        swapNode(i, parent(i));
        i = parent(i);
    }
}

