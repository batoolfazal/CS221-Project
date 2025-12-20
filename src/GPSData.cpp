#include "GPSData.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GPSCoordinate::GPSCoordinate(double lat, double lon, double alt)
    : latitude(lat), longitude(lon), altitude(alt) {}

double GPSCoordinate::distanceTo(const GPSCoordinate& other) const {
    // Haversine formula for great-circle distance
    double lat1 = latitude * M_PI / 180.0;
    double lat2 = other.latitude * M_PI / 180.0;
    double lon1 = longitude * M_PI / 180.0;
    double lon2 = other.longitude * M_PI / 180.0;
    
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    
    double a = std::sin(dlat/2) * std::sin(dlat/2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon/2) * std::sin(dlon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    // Earth radius in meters
    double earthRadius = 6371000.0;
    double distance = earthRadius * c;
    
    // Add altitude difference
    double altDiff = std::abs(altitude - other.altitude);
    return std::sqrt(distance * distance + altDiff * altDiff);
}

void GPSCoordinate::display() const {
    std::cout << "GPS: (" << latitude << ", " << longitude 
              << ") Alt: " << altitude << "m" << std::endl;
}

