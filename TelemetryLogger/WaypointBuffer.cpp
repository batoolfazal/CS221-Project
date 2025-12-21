#include "WaypointBuffer.h"
#include <iostream>

WaypointNode::WaypointNode(double lat, double lon, double alt) 
    : latitude(lat), longitude(lon), altitude(alt), next(nullptr) {}

WaypointBuffer::WaypointBuffer() 
    : head(nullptr), tail(nullptr), count(0) {}

WaypointBuffer::~WaypointBuffer() {
    WaypointNode* current = head;
    while (current != nullptr) {
        WaypointNode* next = current->next;
        delete current;
        current = next;
    }
}

void WaypointBuffer::Insert(double lat, double lon, double alt) {
    WaypointNode* newNode = new WaypointNode(lat, lon, alt);
    
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    
    count++;
}

void WaypointBuffer::Display() {
    std::cout << "Waypoint Buffer (" << count << " points):\n";
    
    WaypointNode* current = head;
    int index = 0;
    
    while (current != nullptr) {
        std::cout << "  [" << index << "] "
             << "Lat: " << current->latitude << ", "
             << "Lon: " << current->longitude << ", "
             << "Alt: " << current->altitude << "m\n";
        current = current->next;
        index++;
    }
}

int WaypointBuffer::getCount() const {
    return count;
}

WaypointNode* WaypointBuffer::getHead() const {
    return head;
}

