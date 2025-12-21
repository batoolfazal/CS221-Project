#include "MotorControl.h"

namespace MotorControl {
    double calculatePWM(double base, double rollCorrection, double pitchCorrection) {
        double pwm = base + rollCorrection + pitchCorrection;
        
        // Clamp to valid range
        if (pwm < 0.0) pwm = 0.0;
        if (pwm > 100.0) pwm = 100.0;
        
        return pwm;
    }
    
    bool isSafePWM(double pwm) {
        return pwm >= 0.0 && pwm <= 100.0;
    }
    
    void emergencyStop(double& pwm) {
        pwm = 0.0;
    }
}

