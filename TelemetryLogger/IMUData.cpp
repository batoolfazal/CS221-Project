#include "IMUData.h"
#include <iostream>
#include <cmath>

IMUData::IMUData(double r, double p, double y)
    : roll(r), pitch(p), yaw(y) {}

void IMUData::display() const {
    std::cout << "IMU: Roll=" << roll << "° Pitch=" << pitch 
              << "° Yaw=" << yaw << "°" << std::endl;
}

bool IMUData::isLevel() const {
    const double LEVEL_THRESHOLD = 5.0; // degrees
    return std::abs(roll) < LEVEL_THRESHOLD && 
           std::abs(pitch) < LEVEL_THRESHOLD;
}

