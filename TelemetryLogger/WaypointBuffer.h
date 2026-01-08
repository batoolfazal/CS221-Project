#ifndef WAYPOINT_BUFFER_H
#define WAYPOINT_BUFFER_H

// Simple waypoint buffer using singly linked list
struct WaypointNode {
    double latitude;
    double longitude;
    double altitude;
    WaypointNode* next;
    
    WaypointNode(double lat, double lon, double alt);
};

class WaypointBuffer {
private:
    WaypointNode* head;
    WaypointNode* tail;
    int count;
    
public:
    WaypointBuffer();
    ~WaypointBuffer();
    
    void Insert(double lat, double lon, double alt);
    void Display();
    
    int getCount() const;
    WaypointNode* getHead() const;
};

#endif // WAYPOINT_BUFFER_H

