#ifndef IMU_DATA_H
#define IMU_DATA_H

// IMU (Inertial Measurement Unit) data
struct IMUData {
    double roll;
    double pitch;
    double yaw;
    
    IMUData(double r = 0.0, double p = 0.0, double y = 0.0);
    void display() const;
    bool isLevel() const;
};

#endif // IMU_DATA_H

