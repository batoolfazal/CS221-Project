#ifndef DRONE_LOGGER_H
#define DRONE_LOGGER_H

#include <string>
#include <vector>
#include <unordered_map>

// Flight data structure with DOUBLY LINKED LIST
struct FlightData {
    double timestamp;
    double latitude;
    double longitude;
    double altitude;
    double roll;
    double pitch;
    double yaw;
    double vel_x;
    double vel_y;
    double vel_z;
    double battery;
    double signal;
    
    // DOUBLY LINKED LIST POINTERS
    FlightData* next;
    FlightData* prev;
    
    FlightData();
};

// Main telemetry logger class
class DroneLogger {
private:
    FlightData* head;
    FlightData* tail;
    std::unordered_map<int, FlightData*> timestampCache;
    int dataCount;
    std::string logFilename;
    
public:
    DroneLogger(std::string filename = "DroneFlightLog.csv");
    ~DroneLogger();
    
    void ReadFromFile();
    FlightData* getLatestReading();
    std::vector<FlightData> getLastNReadings(int N);
    
    double MaxAltitude();
    double AverageVelocity();
    double TotalDistance();
    
    void DisplayAll();
    void DisplayReverse(int N = 10);
    
    int getDataCount() const;
};

#endif // DRONE_LOGGER_H

