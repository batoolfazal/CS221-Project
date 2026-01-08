#include "Quadcopter.h"
#include <iostream>

Motor::Motor(int id) 
    : motorID(id), currentPWM(0), targetPWM(0), isActive(false) {}

void Motor::setPWM(double pwm) {
    if (pwm < 0) pwm = 0;
    if (pwm > 100) pwm = 100;
    targetPWM = pwm;
    currentPWM = pwm;
}

void Motor::display() {
    std::cout << "Motor " << motorID << ": " 
         << currentPWM << "% PWM "
         << (isActive ? "[ACTIVE]" : "[IDLE]") << "\n";
}

QuadcopterSystem::QuadcopterSystem(int num) : numMotors(num) {
    for (int i = 0; i < numMotors; i++) {
        motors.push_back(Motor(i + 1));
    }
    std::cout << "🚁 Quadcopter initialized with " << numMotors << " motors\n";
}

void QuadcopterSystem::armMotors() {
    std::cout << "🔧 Arming motors...\n";
    for (auto& motor : motors) {
        motor.isActive = true;
        motor.setPWM(10.0);
    }
}

void QuadcopterSystem::disarmMotors() {
    std::cout << "🔒 Disarming motors...\n";
    for (auto& motor : motors) {
        motor.isActive = false;
        motor.setPWM(0.0);
    }
}

void QuadcopterSystem::stabilize(double roll, double pitch) {
    double basePWM = 50.0;
    double rollCorrection = roll * 0.5;
    double pitchCorrection = pitch * 0.5;
    
    if (motors.size() >= 4) {
        motors[0].setPWM(basePWM - rollCorrection - pitchCorrection);  // FL
        motors[1].setPWM(basePWM + rollCorrection - pitchCorrection);  // FR
        motors[2].setPWM(basePWM + rollCorrection + pitchCorrection);  // RR
        motors[3].setPWM(basePWM - rollCorrection + pitchCorrection);  // RL
    }
}

void QuadcopterSystem::displayStatus() {
    std::cout << "\n--- Quadcopter Status ---\n";
    for (auto& motor : motors) {
        motor.display();
    }
    std::cout << "\n";
}

