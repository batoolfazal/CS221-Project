#ifndef QUADCOPTER_H
#define QUADCOPTER_H

#include <vector>

// Individual motor
struct Motor {
    int motorID;
    double currentPWM;
    double targetPWM;
    bool isActive;
    
    Motor(int id);
    void setPWM(double pwm);
    void display();
};

// Quadcopter control system
class QuadcopterSystem {
private:
    std::vector<Motor> motors;
    int numMotors;
    
public:
    QuadcopterSystem(int num = 4);
    
    void armMotors();
    void disarmMotors();
    void stabilize(double roll, double pitch);
    void displayStatus();
};

#endif // QUADCOPTER_H

