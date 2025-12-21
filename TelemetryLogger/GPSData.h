#ifndef GPS_DATA_H
#define GPS_DATA_H

// GPS coordinate structure
struct GPSCoordinate {
    double latitude;
    double longitude;
    double altitude;
    
    GPSCoordinate(double lat = 0.0, double lon = 0.0, double alt = 0.0);
    double distanceTo(const GPSCoordinate& other) const;
    void display() const;
};

#endif // GPS_DATA_H

