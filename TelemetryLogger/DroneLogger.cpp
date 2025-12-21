#include "DroneLogger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

FlightData::FlightData() 
    : timestamp(0), latitude(0), longitude(0), altitude(0),
      roll(0), pitch(0), yaw(0),
      vel_x(0), vel_y(0), vel_z(0),
      battery(0), signal(0),
      next(nullptr), prev(nullptr) {}

DroneLogger::DroneLogger(std::string filename) 
    : head(nullptr), tail(nullptr), dataCount(0), logFilename(filename) {
    std::cout << "📋 DroneLogger initialized for file: " << filename << std::endl;
}

DroneLogger::~DroneLogger() {
    FlightData* current = head;
    while (current != nullptr) {
        FlightData* next = current->next;
        delete current;
        current = next;
    }
}

void DroneLogger::ReadFromFile() {
    std::ifstream file(logFilename);
    
    if (!file.is_open()) {
        std::cerr << "❌ Error: Cannot open " << logFilename << std::endl;
        return;
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    std::cout << "📖 Reading flight data..." << std::endl;
    
    while (std::getline(file, line)) {
        FlightData* data = new FlightData();
        
        std::stringstream ss(line);
        std::string value;
        
        std::getline(ss, value, ','); data->timestamp = std::stod(value);
        std::getline(ss, value, ','); data->latitude = std::stod(value);
        std::getline(ss, value, ','); data->longitude = std::stod(value);
        std::getline(ss, value, ','); data->altitude = std::stod(value);
        std::getline(ss, value, ','); data->roll = std::stod(value);
        std::getline(ss, value, ','); data->pitch = std::stod(value);
        std::getline(ss, value, ','); data->yaw = std::stod(value);
        std::getline(ss, value, ','); data->vel_x = std::stod(value);
        std::getline(ss, value, ','); data->vel_y = std::stod(value);
        std::getline(ss, value, ','); data->vel_z = std::stod(value);
        std::getline(ss, value, ','); data->battery = std::stod(value);
        std::getline(ss, value, ','); data->signal = std::stod(value);
        
        // Insert at tail (DOUBLY LINKED LIST)
        if (head == nullptr) {
            head = tail = data;
        } else {
            data->prev = tail;
            tail->next = data;
            tail = data;
        }
        
        timestampCache[static_cast<int>(data->timestamp)] = data;
        dataCount++;
    }
    
    file.close();
    std::cout << "✅ Loaded " << dataCount << " flight data points" << std::endl;
}

FlightData* DroneLogger::getLatestReading() {
    return tail;
}

std::vector<FlightData> DroneLogger::getLastNReadings(int N) {
    std::vector<FlightData> result;
    
    if (tail == nullptr) return result;
    
    FlightData* current = tail;
    int count = 0;
    
    // Traverse backward using PREV pointer
    while (current != nullptr && count < N) {
        result.push_back(*current);
        current = current->prev;
        count++;
    }
    
    std::reverse(result.begin(), result.end());
    return result;
}

double DroneLogger::MaxAltitude() {
    double maxAlt = 0.0;
    FlightData* current = head;
    
    while (current != nullptr) {
        if (current->altitude > maxAlt) {
            maxAlt = current->altitude;
        }
        current = current->next;
    }
    
    return maxAlt;
}

double DroneLogger::AverageVelocity() {
    double sum = 0.0;
    int count = 0;
    FlightData* current = head;
    
    while (current != nullptr) {
        double vel = std::sqrt(
            current->vel_x * current->vel_x +
            current->vel_y * current->vel_y +
            current->vel_z * current->vel_z
        );
        sum += vel;
        count++;
        current = current->next;
    }
    
    return (count > 0) ? sum / count : 0.0;
}

double DroneLogger::TotalDistance() {
    double totalDist = 0.0;
    FlightData* current = head;
    
    while (current != nullptr && current->next != nullptr) {
        double lat1 = current->latitude;
        double lon1 = current->longitude;
        double lat2 = current->next->latitude;
        double lon2 = current->next->longitude;
        
        double dlat = (lat2 - lat1) * 111000;
        double dlon = (lon2 - lon1) * 111000 * std::cos(lat1 * M_PI / 180.0);
        
        double dist = std::sqrt(dlat*dlat + dlon*dlon);
        totalDist += dist;
        
        current = current->next;
    }
    
    return totalDist;
}

void DroneLogger::DisplayAll() {
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "COMPLETE FLIGHT LOG\n";
    std::cout << std::string(100, '=') << "\n";
    
    std::cout << std::setw(12) << "Time"
         << std::setw(12) << "Lat"
         << std::setw(12) << "Lon"
         << std::setw(10) << "Alt(m)"
         << std::setw(8) << "Roll"
         << std::setw(8) << "Pitch"
         << std::setw(8) << "Yaw"
         << std::setw(10) << "Battery"
         << "\n";
    std::cout << std::string(100, '-') << "\n";
    
    FlightData* current = head;
    while (current != nullptr) {
        std::cout << std::fixed << std::setprecision(1);
        std::cout << std::setw(12) << (int)current->timestamp
             << std::setprecision(6)
             << std::setw(12) << current->latitude
             << std::setw(12) << current->longitude
             << std::setprecision(1)
             << std::setw(10) << current->altitude
             << std::setw(8) << current->roll
             << std::setw(8) << current->pitch
             << std::setw(8) << current->yaw
             << std::setw(10) << current->battery
             << "\n";
        current = current->next;
    }
    
    std::cout << std::string(100, '=') << "\n";
}

void DroneLogger::DisplayReverse(int N) {
    std::cout << "\n=== REVERSE TRAVERSAL (Last " << N << " readings) ===\n";
    
    FlightData* current = tail;
    int count = 0;
    
    while (current != nullptr && count < N) {
        std::cout << "T=" << (int)current->timestamp 
             << " Alt=" << current->altitude << "m"
             << " Battery=" << current->battery << "%\n";
        current = current->prev;  // DOUBLY LINKED LIST FEATURE
        count++;
    }
    
    std::cout << "\n";
}

int DroneLogger::getDataCount() const {
    return dataCount;
}

