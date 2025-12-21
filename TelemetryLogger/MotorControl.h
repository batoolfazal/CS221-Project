#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Motor control utility functions
namespace MotorControl {
    double calculatePWM(double base, double rollCorrection, double pitchCorrection);
    bool isSafePWM(double pwm);
    void emergencyStop(double& pwm);
}

#endif // MOTOR_CONTROL_H

